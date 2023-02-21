#include <iostream>
#include <bitset>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <typeinfo>

uint16_t packet_magic = 0x11ad;

struct pair_8
{
	uint8_t one;
	uint8_t two;	
};

struct pair_16
{
	uint16_t one;
	uint16_t two;
};

union pair_from_16
{
	uint16_t input;
	pair_8 output;
};

enum ReplyModes
{
	ASK_REPLY = 0,
	NO_REPLY
};

enum DeliverModes
{
	DIRECT = 0,
	BROADCAST
};

enum Signals
{
	TELEMETRY = 0,
	CONTROL,
	SOS,
	ABORT,
	RESEND,
	CONFIRM,
	HEARTBEAT
};

uint16_t Fletcher16(uint8_t *data, uint32_t count)
{
	uint16_t sum1 = 0;
	uint16_t sum2 = 0;

	for (uint32_t index = 0; index < count; ++index)
	{
		sum1 = (sum1 + data[index]) % 255;
		sum2 = (sum2 + sum1) % 255;
	}

	return (sum2 << 8) | sum1;
}

void Fletcher16Pair(uint8_t data, pair_16 *pair)
{
	pair->one = (pair->one + data) % 255;
	pair->two = (pair->two + pair->one) % 255;
}

struct Header
{
	unsigned reserved:     2;
	unsigned signal:       4;
	unsigned receiver:     4;
	unsigned sender:       4;
	unsigned deliver_mode: 1;
	unsigned reply_mode:   1;
} __attribute__((packed));

union HeaderUnion
{
	uint16_t *p;
	Header *raw;
};

struct Packet
{
	uint16_t checksum;
	uint16_t header;
	uint16_t magic;
} __attribute__((packed));

union PacketUnion
{
	uint64_t *p;
	Packet *raw;
};

void print_string_as_hex(uint8_t *str, uint32_t length)
{
	std::cout << "0x";
	for (uint32_t i = 0; i < length; i++)
	{
		printf("%02x", str[i]);
	}
	std::cout << '\n';
}

int32_t main()
{
	Header header;
	HeaderUnion header_string;
	
	header.reserved     = 0;
	header.signal       = Signals::ABORT;
	header.receiver     = 0x01;
	header.sender       = 0x00;
	header.deliver_mode = DeliverModes::DIRECT;
	header.reply_mode   = ReplyModes::NO_REPLY;
	
	header_string.raw = &header;
	
	std::bitset<16> header_set(*header_string.p);
	
	std::cout << header_set << '\n';
	
	// Packet packet;
	uint8_t packet_string[262];
	// uint8_t *packet_string;
	// char *packet_string;
	
	pair_from_16 magic_pair;
	magic_pair.input = packet_magic;
	
	pair_from_16 header_pair;
	header_pair.input = *header_string.p;
	
	pair_from_16 checksum_pair;
	
	// strncpy(packet_string, (char *)&magic_pair.output.two, 1);
	// strncat(packet_string, (char *)&magic_pair.output.one, 1);
	packet_string[0] = magic_pair.output.one;
	packet_string[1] = magic_pair.output.two;
	packet_string[2] = header_pair.output.one;
	packet_string[3] = header_pair.output.two;
	packet_string[4] = '\0';
	packet_string[5] = '\0';
	packet_string[6] = '\x02';
	packet_string[7] = 'h';
	packet_string[8] = 'i';
	
	pair_16 sum_pair;
	sum_pair.one = 0;
	sum_pair.two = 0;
	
	for (uint32_t i = 0; i < 7; i++)
	{
		// printf("%02x\n", packet_string[i]);
		Fletcher16Pair(packet_string[i], &sum_pair);
		// printf("%02x%02x\n", sum_pair.one, sum_pair.two);
	}
	checksum_pair.input = (sum_pair.two << 8) | sum_pair.one;
	
	packet_string[4] = checksum_pair.output.one;
	packet_string[5] = checksum_pair.output.two;
	
	// std::cout << packet_string << '\n';
	// printf("0x%02x\n", magic_pair.output.one);
	print_string_as_hex(packet_string, 7);
	
	return 0;
}
