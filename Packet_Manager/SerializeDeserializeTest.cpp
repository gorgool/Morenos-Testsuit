#include "SearchResult_Msg.h"
#include <iostream>

void SerializeDeserializaTest()
{
  // Test case #1
  {
    PlotDescriptionRaw msg
		{
			{
				static_cast<std::uint8_t>(245),
				static_cast<std::uint8_t>(0),
				static_cast<std::uint8_t>(0),
				static_cast<std::uint8_t>(0),
				static_cast<std::uint8_t>(0)
			},
			1154,
			421,
			110,
            123,
			2,
			12453,
			956,
			23
		};

    unsigned char buf[1500];
    if (PlotDescriptionRaw::msg_size != serialize(msg, buf))
    {
      std::cout << "Serialize Deserialize Test #1 - FAIL.\n";
      return;
    }

    PlotDescriptionRaw ret;
    deserialize(buf, ret);

    bool fail = false;

    if (msg.channel_id != ret.channel_id)
      fail = true;

    if (msg.freq_range_start != ret.freq_range_start)
      fail = true;

    if (msg.freq_range_width != ret.freq_range_width)
      fail = true;

    if (msg.power != ret.power)
      fail = true;

    if (msg.u != ret.u)
      fail = true;

    if (msg.v != ret.v)
      fail = true;

    for (std::size_t idx = 0; idx < 5; ++idx)
    {
      if (msg.referance_time[idx] != ret.referance_time[idx])
        fail = true;
    }

    if(fail)
    {
      std::cout << "Serialize Deserialize Test #1 - FAIL.\n";
      return;
    }
  }

  // Test case #2
  {
    // Size = 29 + 2*34 = 97
    const std::uint16_t msg_size = 97;
    SearchResult_MsgRaw msg
		{
			{
				static_cast<std::uint8_t>(245),
				static_cast<std::uint8_t>(12),
				static_cast<std::uint8_t>(121),
				static_cast<std::uint8_t>(0),
				static_cast<std::uint8_t>(0)
			},
			{
				static_cast<std::uint8_t>(245),
				static_cast<std::uint8_t>(154),
				static_cast<std::uint8_t>(124),
				static_cast<std::uint8_t>(0),
				static_cast<std::uint8_t>(0)
			},
			1,
			{ 123, 0, 11, 114, 239, 0, 11, 122 },
			110,
			2,
			{
				PlotDescriptionRaw
				{
					{
						static_cast<std::uint8_t>(245),
						static_cast<std::uint8_t>(100),
						static_cast<std::uint8_t>(0),
						static_cast<std::uint8_t>(0),
						static_cast<std::uint8_t>(0)
					},
					1154,
					421,
					110,
          123,
					2,
					12453,
					956,
					23
				},
				PlotDescriptionRaw
				{
					{
						static_cast<std::uint8_t>(245),
						static_cast<std::uint8_t>(100),
						static_cast<std::uint8_t>(1),
						static_cast<std::uint8_t>(0),
						static_cast<std::uint8_t>(0)
					},
					1154,
					4021,
					1010,
          123,
					8,
					12530,
					56,
					3
				},
			}
		};

    unsigned char buf[1500];
    if (serialize(msg, buf) != msg_size)
    {
      std::cout << "Serialize Deserialize Test #2 - FAIL.\n";
      return;
    }

    if (msg.size() != msg_size)
    {
      std::cout << "Serialize Deserialize Test #2 - FAIL.\n";
      return;
    }

    SearchResult_MsgRaw ret;
    deserialize(buf, ret);

    bool fail = false;

    if(msg.plots_count != ret.plots_count)
      fail = true;

    if(msg.polarization_type != ret.polarization_type)
      fail = true; 
    
    if(msg.search_area_id != ret.search_area_id)
      fail = true; 
    
    for (std::size_t idx = 0; idx < 5; ++idx)
    {
      if (msg.process_time[idx] != ret.process_time[idx])
        fail = true;
    }

    for (std::size_t idx = 0; idx < 5; ++idx)
    {
      if (msg.transfer_time[idx] != ret.transfer_time[idx])
        fail = true;
    }

    for (std::size_t idx = 0; idx < 8; ++idx)
    {
      if (msg.signal_amp[idx] != ret.signal_amp[idx])
        fail = true;
    }

    for(std::size_t plot_idx = 0; plot_idx < 2; ++plot_idx)
    {
      auto& msg_plot = msg.p[plot_idx];
      auto& ret_plot = ret.p[plot_idx];

      if (msg_plot.channel_id != ret_plot.channel_id)
      fail = true;

      if (msg_plot.freq_range_start != ret_plot.freq_range_start)
        fail = true;

      if (msg_plot.freq_range_width != ret_plot.freq_range_width)
        fail = true;

      if (msg_plot.power != ret_plot.power)
        fail = true;

      if (msg_plot.u != ret_plot.u)
        fail = true;

      if (msg_plot.v != ret_plot.v)
        fail = true;

      for (std::size_t idx = 0; idx < 5; ++idx)
      {
        if (msg_plot.referance_time[idx] != ret_plot.referance_time[idx])
          fail = true;
      }
    }

    if(fail)
    {
      std::cout << "Serialize Deserialize Test #2 - FAIL.\n";
      return;
    }
  }

  std::cout << "Serialize Deserialize Test - SUCCESS.\n";
}
