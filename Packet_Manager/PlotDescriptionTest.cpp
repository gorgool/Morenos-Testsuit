#include "SearchResult_Msg.h"
#include <iostream>

void PlotDescriptionTest()
{
  // Test case #1
  {
    PlotDescription msg
    {
      1254631156,
      0.256,
      -0.444,
      0.125,
      4,
      15236,
      10.52,
      0.45
    };

    if (validate(msg) == false)
    {
      std::cout << "Plot description validation tests - FAIL.\n";
      return;
    }
  }

  // Test case #2
  {
    PlotDescription msg
    {
      1254631156,
      1.256,
      -0.444,
      0.125,
      4,
      15236,
      10.52,
      0.45
    };

    if (validate(msg) == true)
    {
      std::cout << "Plot description validation tests - FAIL.\n";
      return;
    }
  }

  // Test case #3
  {
    PlotDescription msg
    {
      1254631156,
      0.256,
      -1.444,
      0.125,
      4,
      15236,
      10.52,
      0.45
    };

    if (validate(msg) == true)
    {
      std::cout << "Plot description validation tests - FAIL.\n";
      return;
    }
  }

  // Test case #4
  {
    PlotDescription msg
    {
      1254631156,
      0.256,
      -0.444,
      1.125,
      4,
      15236,
      10.52,
      0.45
    };

    if (validate(msg) == true)
    {
      std::cout << "Plot description validation tests - FAIL.\n";
      return;
    }
  }

  // Test case #5
  {
    PlotDescription msg
    {
      1254631156,
      0.256,
      -0.444,
      0.125,
      9,
      15236,
      10.52,
      0.45
    };

    if (validate(msg) == true)
    {
      std::cout << "Plot description validation tests - FAIL.\n";
      return;
    }
  }

  // Test case #6
  {
    PlotDescription msg
    {
      1254631156,
      0.256,
      -0.444,
      0.125,
      0,
      15236,
      10.52,
      0.45
    };

    if (validate(msg) == true)
    {
      std::cout << "Plot description validation tests - FAIL.\n";
      return;
    }
  }

  // Test case #7
  {
    PlotDescription msg
    {
      1254631156,
      0.256,
      -0.444,
      0.125,
      4,
      35236,
      10.52,
      0.45
    };

    if (validate(msg) == true)
    {
      std::cout << "Plot description validation tests - FAIL.\n";
      return;
    }
  }

  // Test case #8
  {
    PlotDescription msg
    {
      1254631156,
      0.256,
      -0.444,
      0.125,
      4,
      15236,
      -1.52,
      0.45
    };

    if (validate(msg) == true)
    {
      std::cout << "Plot description validation tests - FAIL.\n";
      return;
    }
  }

  // Test case #9
  {
    PlotDescription msg
    {
      1254631156,
      0.256,
      -0.444,
      0.125,
      4,
      15236,
      1.52,
      1000.45
    };

    if (validate(msg) == true)
    {
      std::cout << "Plot description validation tests - FAIL.\n";
      return;
    }
  }

  std::cout << "Plot description validation tests - SUCCESS.\n";
}