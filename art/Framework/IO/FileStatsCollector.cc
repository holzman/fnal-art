#include "art/Framework/IO/FileStatsCollector.h"
#include "boost/date_time/posix_time/posix_time.hpp"

#include <string>

art::FileStatsCollector::
FileStatsCollector(std::string const & moduleLabel,
                     std::string const & processName)
  :
  moduleLabel_(moduleLabel),
  processName_(processName),
  lowestSubRun_(),
  highestSubRun_(),
  fo_(),
  fc_(),
  seqNo_(0ul),
  lastOpenedInputFile_(),
  inputFilesSeen_(),
  nEvents_(0ul),
  subRunsSeen_()
{
}

void
art::FileStatsCollector::
recordFileOpen()
{
  reset_(); // Reset statistics.
  if (!inputFilesSeen_.empty()) {
    inputFilesSeen_.emplace_back(lastOpenedInputFile_);
  }
  fo_ = boost::posix_time::second_clock::universal_time();
}

void
art::FileStatsCollector::
recordInputFile(std::string const & inputFileName)
{
  if (!inputFileName.empty()) {
    inputFilesSeen_.emplace_back(inputFileName);
  }
  lastOpenedInputFile_ = inputFileName;
}

void
art::FileStatsCollector::
recordEvent(EventID const & id)
{
  // Currently not recording by event.
  recordSubRun(id.subRunID());
  ++nEvents_;
}

void
art::FileStatsCollector::
recordRun(RunID const & id)
{
  if ((!lowestSubRun_.runID().isValid()) ||
      id < lowestSubRun_.runID()) {
    lowestSubRun_ = SubRunID::invalidSubRun(id);
  }
  if (id > highestSubRun_.runID()) {
    // Sort-invalid-first gives the correct answer.
    highestSubRun_ = SubRunID::invalidSubRun(id);
  }
}

void
art::FileStatsCollector::
recordSubRun(SubRunID const & id)
{
  recordRun(id.runID());
  if (id.runID() == lowestSubRun_.runID() &&
      (id.isValid() &&
       ((!lowestSubRun_.isValid()) || // No valid subrun yet.
        id < lowestSubRun_))) {
    lowestSubRun_ = id;
  }
  if (id > highestSubRun_) {
    // Sort-invalid-first gives the correct answer.
    highestSubRun_ = id;
  }
  subRunsSeen_.emplace(id);
}

void
art::FileStatsCollector::
recordFileClose()
{
  fc_ =  boost::posix_time::second_clock::universal_time();
  ++seqNo_;
}

void
art::FileStatsCollector::
reset_()
{
  fo_ =
    fc_ = boost::posix_time::ptime();
  lowestSubRun_ = SubRunID();
  highestSubRun_ = SubRunID();
  inputFilesSeen_.clear();
  nEvents_ = 0ul;
  subRunsSeen_.clear();
}