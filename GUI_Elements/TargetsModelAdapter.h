#pragma once

#include "PacketHeaders.h"
#include "SearchResult_Msg.h"
#include "AntennaParamsModel.h"
#include "TargetsModel.h"

class TargetsModelAdapter
{
public:

  TargetsModelAdapter();
  TargetsModelAdapter(const SearchResult_Msg& msg, const int id, TargetType type);
  void update(const SearchResult_Msg& msg, const int id, TargetType type);
  TargetsModel* get_model();

private:

  TargetsModel model_;

};
