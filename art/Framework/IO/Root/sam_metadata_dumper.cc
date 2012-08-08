// sam_metadata_dumper.cc

#include "cetlib/container_algorithms.h"

#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/make_ParameterSet.h"
#include "art/Persistency/Provenance/ParameterSetMap.h"
#include "art/Persistency/Provenance/ParameterSetBlob.h"

#include "boost/program_options.hpp"

#include "art/Framework/Core/RootDictionaryManager.h"
#include "art/Framework/IO/Root/GetFileFormatEra.h"
#include "art/Framework/IO/Root/rootNames.h"
#include "art/Framework/IO/Root/setMetaDataBranchAddress.h"
#include "art/Persistency/Provenance/FileFormatVersion.h"
#include "art/Persistency/RootDB/SQLite3Wrapper.h"
#include "art/Persistency/RootDB/tkeyvfs.h"

#include "TFile.h"

extern "C" {
#include "sqlite3.h"
}

#include <iostream>
#include <iterator>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

namespace bpo = boost::program_options;

using std::back_inserter;
using std::cerr;
using std::cout;
using std::endl;
using std::ostream;
using std::string;
using std::vector;
using fhicl::ParameterSet;
using art::ParameterSetBlob;
using art::ParameterSetMap;

typedef vector<string> stringvec;
struct SamMetadataEntry
{
  int SMDid;
  std::string name;
  std::string value;
};

// Print the human-readable form of a ParameterSet from which we strip
// the "module_label" parameter.
void print_one_sam_metadata_entry(SamMetadataEntry const & ent,
          size_t idLen,
          size_t longestName,
          ostream & output)
{
//  std::cerr << "---> print_one_sam_metadata_entry \n";
  const size_t maxIDdigits = 5;
  const size_t maxNameSpacing = 20;

  // right-justify SMDid (unless it is more than 5 digits)
  int id = static_cast<int>(ent.SMDid);
  size_t maxIDspace = std::min(idLen,maxIDdigits);
  int nspaces = maxIDspace - 1;
  for (int i=0; (nspaces>0) && (id >0); ++i) {
    id /= 10;
    if (id > 0) --nspaces;
  }
  for (int i = 0; i<nspaces; ++i) output << " ";
  output << ent.SMDid << ": ";

  output << ent.name;

  // right-justify value (unless name is more than 20 characters)
  size_t nameSpacing = maxNameSpacing;
  if (longestName < maxNameSpacing) nameSpacing = longestName;
  nspaces = static_cast<int> (nameSpacing - ent.name.size());
  while (nspaces > 0) {
    output << " ";
    --nspaces;
  }

  output << " " << ent.value << "\n";
}

// Print all the entries in the sam metadata from a file
void print_all_sam_metadata_entries(vector<SamMetadataEntry> const & entries,
                                  ostream & output,
                                  ostream & /*errors*/)
{
//  std::cerr << "---> print_all_sam_metadata_entries \n";
  // For nice formatting, determine maximum id lenght and name size,
  // so that values can be lined up.
  int maxID = 1;
  size_t longestName = 1;
  for (size_t i = 0; i < entries.size(); ++i) {
    if (entries[i].SMDid > maxID) maxID = entries[i].SMDid;
    if (entries[i].name.size() > longestName)
          longestName = entries[i].name.size();
  }
  size_t idLen = 1;
  for (int i=0; (i<5) && (maxID >0); ++i) {
    maxID /= 10;
    if (maxID > 0) ++idLen;
  }
  for (size_t i = 0; i < entries.size(); ++i) {
    print_one_sam_metadata_entry(entries[i], idLen, longestName, output);
  }
}

// Read all the sam metadata entries stored in the table in 'file'.
// Write any error messages to errors.
// Return false on failure, and true on success.
bool read_all_sam_metadata_entries(
      TFile & file,
                        vector<SamMetadataEntry>& all_metadata_entries,
                        ostream & errors)
{
//  std::cerr << "---> read_all_sam_metadata_entries \n";
  SamMetadataEntry ent;
  // Open the DB
  art::SQLite3Wrapper sqliteDB(&file, "RootFileDB");
  // Read the entries into memory.
  sqlite3_stmt * stmt = 0;
  sqlite3_prepare_v2(sqliteDB,
    "SELECT ID, Name, Value from SAM_metadata;", -1, &stmt, NULL);
  bool row_found = false;
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    row_found = true;
    ent.SMDid = sqlite3_column_int (stmt, 0);
    ent.name  = std::string(
      reinterpret_cast<char const *>(sqlite3_column_text(stmt, 1)) );
    ent.value = std::string(
      reinterpret_cast<char const *>(sqlite3_column_text(stmt, 2)) );
    all_metadata_entries.push_back(ent);
  }
  if (!row_found) {
     errors << "No SAM Metadata rows found - table is missing or empty\n";
     return false;
  }
  return true;
}

// Extract the sam metadata from the given TFile.
// The metadata entries are written to the stream output, and
// error messages are written to the stream errors.
//
// Returns 0 to indicate success, and 1 on failure.
// Precondition: file.IsZombie() == false

// Caution: We pass 'file' by non-const reference because the TFile interface
// does not declare the functions we use to be const, even though they do not
// modify the underlying file.
int print_sam_metadata_from_file(TFile & file,
                                 ostream & output,
                                 ostream & errors)
{
//  std::cerr << "---> print_sam_metadata_from_file \n";
  vector<SamMetadataEntry> all_metadata_entries;
  if (! read_all_sam_metadata_entries(file, all_metadata_entries, errors)) {
    errors << "Unable to to read metadata entries.\n";
    return 1;
  }
  // Iterate through all the entries, printing each one.
  print_all_sam_metadata_entries(all_metadata_entries, output, errors);
  return 0;
}

// Extract all the requested metadata tables (for from the named files.
// The contents of the tables are written to the stream output, and
// error messages are written to the stream errors.
//
// The return value is the number of files in which errors were
// encountered, and is thus 0 to indicate success.
int print_sam_metadata_from_files(
         stringvec const & file_names,
                           ostream & output,
                           ostream & errors)
{
//  std::cerr << "---> print_sam_metadata_from_files \n";
  int rc = 0;
  for (stringvec::const_iterator
       i = file_names.begin(),
       e = file_names.end();
       i != e;
       ++i) {
    TFile current_file(i->c_str(), "READ");
    if (current_file.IsZombie()) {
      ++rc;
      errors << "Unable to open file '"
             << *i
             << "' for reading."
             << "\nSkipping to next file.\n";
    }
    else {
      output << "\nSAM metadata from file " << *i  << ":\n\n";
      rc += print_sam_metadata_from_file(current_file, output, errors);
      output << "-------------------------------\n";
    }
  }
  return rc;
}


int main(int argc, char * argv[])
{
  // ------------------
  // use the boost command line option processing library to help out
  // with command line options
  std::ostringstream descstr;
  descstr << argv[0]
          << " <options> [<source-file>]+";
  bpo::options_description desc(descstr.str());
  desc.add_options()
  ("help,h",                             "produce help message")
  ("source,s",  bpo::value<stringvec>(), "source data file (multiple OK)");
  bpo::options_description all_opts("All Options");
  all_opts.add(desc);
  // Each non-option argument is interpreted as the name of a files to
  // be processed. Any number of filenames is allowed.
  bpo::positional_options_description pd;
  pd.add("source", -1);
  // The variables_map contains the actual program options.
  bpo::variables_map vm;
  try {
    bpo::store(bpo::command_line_parser(argc, argv).options(all_opts).positional(pd).run(),
               vm);
    bpo::notify(vm);
  }
  catch (bpo::error const & e) {
    std::cerr << "Exception from command line processing in "
              << argv[0] << ": " << e.what() << "\n";
    return 2;
  }
  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return 1;
  }
  // Get the names of the files we will process.
  stringvec file_names;
  size_t file_count = vm.count("source");
  if (file_count < 1) {
    cerr << "One or more input files must be specified;"
         << " supply filenames as program arguments\n"
         << "For usage and options list, please do 'config_dumper --help'.\n";
    return 3;
  }
  file_names.reserve(file_count);
  cet::copy_all(vm["source"].as<stringvec>(),
                std::back_inserter(file_names));
  // Prepare for dealing with Root. We use the RootDictionaryManager to
  // load all necessary dictionaries.
  art::RootDictionaryManager dictionary_loader;

  // Register the tkey VFS with sqlite:
  tkeyvfs_init();

  // Do the work.
  return print_sam_metadata_from_files(file_names,
        cout,
        cerr);
  // Testing.
  //   cout << "Specified module labels\n";
  //   cet::copy_all(module_labels,
  //                 std::ostream_iterator<string>(cout, ", "));
  //   cout << endl;
  //   cout << "Specified process names\n";
  //   cet::copy_all(process_names,
  //                 std::ostream_iterator<string>(cout, ", "));
  //   cout << endl;
  //   cout << "Specified input files\n";
  //   cet::copy_all(file_names,
  //                 std::ostream_iterator<string>(cout, ", "));
  //   cout << endl;
}