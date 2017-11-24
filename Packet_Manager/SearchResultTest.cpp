#include "SearchResult_Msg.h"
#include <iostream>

void SearchResultTest()
{
  // Test case #1
  {
    SearchResult_Msg msg
    {
      1254631156,
      254521,
      0,
      { 0.0, 1.0, 100.0, 115.0, 0.0, .5, 15.0, 46.235 },
      125,
      1,
      { PlotDescription {
        1254631156,
        0.256,
        -0.444,
        0.125,
        4,
        15236,
        10.52,
        0.45
      }}
    };

    if (validate(msg) == false)
    {
      std::cout << "Search result validation tests - FAIL.\n";
      return;
    }
  }

  // Test case #2
  {
    SearchResult_Msg msg
    {
      1254631156,
      254521,
      2,
      { 0.0, 1.0, 100.0, 115.0, 0.0, .5, 15.0, 46.235 },
      125,
      1,
      { PlotDescription{
        1254631156,
        0.256,
        -0.444,
        0.125,
        4,
        15236,
        10.52,
        0.45
    } }
    };

    if (validate(msg) == true)
    {
      std::cout << "Search result validation tests - FAIL.\n";
      return;
    }
  }

  // Test case #3
  {
    SearchResult_Msg msg
    {
      1254631156,
      254521,
      1,
      { 0.0, 1.0, 100.0, 1015.0, 0.0, .5, 15.0, 46.235 },
      125,
      1,
      { PlotDescription{
        1254631156,
        0.256,
        -0.444,
        0.125,
        4,
        15236,
        10.52,
        0.45
    } }
    };

    if (validate(msg) == true)
    {
      std::cout << "Search result validation tests - FAIL.\n";
      return;
    }
  }

  // Test case #4
  {
    SearchResult_Msg msg
    {
      1254631156,
      254521,
      0,
      { 0.0, 1.0, 100.0, 115.0, 0.0, .5, 15.0, 46.235 },
      125,
      2,
      { PlotDescription{
        1254631156,
        0.256,
        -0.444,
        0.125,
        4,
        15236,
        10.52,
        0.45
    } }
    };

    if (validate(msg) == true)
    {
      std::cout << "Search result validation tests - FAIL.\n";
      return;
    }
  }

  // Test case #5
  {
    SearchResult_Msg msg
    {
      1254631156,
      254521,
      0,
      { 0.0, 1.0, 100.0, 115.0, 0.0, .5, 15.0, 46.235 },
      125,
      1,
      { PlotDescription{
        1254631156,
        0.256,
        -0.444,
        0.125,
        9,
        15236,
        10.52,
        0.45
    } }
    };

    if (validate(msg) == true)
    {
      std::cout << "Search result validation tests - FAIL.\n";
      return;
    }
  }

  std::cout << "Search result validation tests - SUCCESS.\n";
}