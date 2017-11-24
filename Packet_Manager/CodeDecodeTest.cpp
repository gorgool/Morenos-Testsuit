#include "SearchResult_Msg.h"
#include <iostream>

void CodeDecodeTest()
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
			2,
			12453,
			956,
			23
		};

		auto ret = decode(msg);

		if (validate(ret) == false)
		{
			std::cout << "Encode - Decode Test - FAIL.\n";
			return;
		}

		auto msg2 = encode(ret);

		bool fail = false;

		if (msg.channel_id != msg2.channel_id)
			fail = true;

		if (msg.freq_range_start != msg2.freq_range_start)
			fail = true;

		if (msg.freq_range_width != msg2.freq_range_width)
			fail = true;

		if (msg.power != msg2.power)
			fail = true;

		if (msg.u != msg2.u)
			fail = true;

		if (msg.v != msg2.v)
			fail = true;

		if (msg.variance != msg2.variance)
			fail = true;

		for (size_t i = 0; i < 5; i++)
		{
			if (msg.referance_time[i] != msg2.referance_time[i])
				fail = true;
		}

		if (fail)
		{
			std::cout << "Encode - Decode Test - FAIL.\n";
			return;
		}
	}
	
	// Test case #2
	{
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
					8,
					12530,
					56,
					3
				},
			}
		};

		auto ret = decode(msg);

		if (validate(ret) == false)
		{
			std::cout << "Encode - Decode Test - FAIL.\n";
			return;
		}

		auto msg2 = encode(ret);

		bool fail = false;

		if (msg.plots_count != msg2.plots_count)
			fail = true;

		if (msg.polarization_type != msg2.polarization_type)
			fail = true;

		if (msg.search_area_id != msg2.search_area_id)
			fail = true;

		for (size_t i = 0; i < 5; i++)
		{
			if (msg.transfer_time[i] != msg2.transfer_time[i])
				fail = true;
		}

		for (size_t i = 0; i < 5; i++)
		{
			if (msg.process_time[i] != msg2.process_time[i])
				fail = true;
		}

		for (size_t i = 0; i < 8; i++)
		{
			if (msg.signal_amp[i] != msg2.signal_amp[i])
				fail = true;
		}

		for (size_t i = 0; i < msg.plots_count; i++)
		{
			if (msg.p[i].channel_id != msg2.p[i].channel_id)
				fail = true;

			if (msg.p[i].freq_range_start != msg2.p[i].freq_range_start)
				fail = true;

			if (msg.p[i].freq_range_width != msg2.p[i].freq_range_width)
				fail = true;

			if (msg.p[i].power != msg2.p[i].power)
				fail = true;

			if (msg.p[i].u != msg2.p[i].u)
				fail = true;

			if (msg.p[i].v != msg2.p[i].v)
				fail = true;

			if (msg.p[i].variance != msg2.p[i].variance)
				fail = true;

			for (size_t j = 0; j < 5; j++)
			{
				if (msg.p[i].referance_time[j] != msg2.p[i].referance_time[j])
					fail = true;
			}
		}

		if (fail)
		{
			std::cout << "Encode - Decode Test - FAIL.\n";
			return;
		}
	}

	std::cout << "Encode - Decode Test - SUCCESS.\n";
}