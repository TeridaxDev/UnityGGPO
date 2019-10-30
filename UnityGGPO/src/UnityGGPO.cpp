#include <stdio.h>
#include <vector>
#include <string>
#include "ggponet.h"
#include "UnityGGPO.h"
#include <memory>
#include <iostream>
#include <string>
#include <cstdio>

constexpr auto PLUGIN_VERSION = "1.0.0.0";
constexpr auto PLUGIN_BUILD_NUMBER = 103;

LogDelegate uggLogCallback = nullptr;

void UggCallLog(const char* text)
{
	if (uggLogCallback) {
		uggLogCallback(text);
	}
}

template<typename ... Args>
void UggCallLogv(const char* format, Args ... args)
{
	size_t size = snprintf(nullptr, 0, format, args ...) + 1; // Extra space for '\0'
	std::unique_ptr<char[]> buf(new char[size]);
	snprintf(buf.get(), size, format, args ...);
	UggCallLog(buf.get());
}

extern "C" const char UNITY_INTERFACE_EXPORT * UNITY_INTERFACE_API UggPluginVersion()
{
	return PLUGIN_VERSION;
}

extern "C" int UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UggPluginBuildNumber()
{
	return PLUGIN_BUILD_NUMBER;
}

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UggSetLogDelegate(LogDelegate callback)
{
	uggLogCallback = callback;
}

extern "C" GGPOSession UNITY_INTERFACE_EXPORT * UNITY_INTERFACE_API UggStartSession(
	BeginGameDelegate beginGame,
	AdvanceFrameDelegate advanceFrame,
	LoadGameStateDelegate loadGameState,
	LogGameStateDelegate logGameState,
	SaveGameStateDelegate saveGameState,
	FreeBufferDelegate freeBuffer,
	OnEventDelegate onEvent,
	const char* game, int num_players, int localport)
{
	UggCallLogv("UggStartSession - %s %i %i", game, num_players, localport);
	GGPOSessionCallbacks cb;
	cb.advance_frame = advanceFrame;
	cb.load_game_state = loadGameState;
	cb.begin_game = beginGame;
	cb.save_game_state = saveGameState;
	cb.load_game_state = loadGameState;
	cb.log_game_state = logGameState;
	cb.free_buffer = freeBuffer;
	cb.on_event = onEvent;

	GGPOSession* ggpo;

	auto ret = ggpo_start_session(&ggpo, &cb, game, num_players, sizeof(uint64_t), localport);

	if (GGPO_SUCCEEDED(ret)) {
		ggpo;
	}
	else {
		return nullptr;
	}
}

extern "C" GGPOSession UNITY_INTERFACE_EXPORT * UNITY_INTERFACE_API UggStartSpectating(
	BeginGameDelegate beginGame,
	AdvanceFrameDelegate advanceFrame,
	LoadGameStateDelegate loadGameState,
	LogGameStateDelegate logGameState,
	SaveGameStateDelegate saveGameState,
	FreeBufferDelegate freeBuffer,
	OnEventDelegate onEvent,
	const char* game, int num_players, int localport, char* host_ip, int host_port)
{
	GGPOSessionCallbacks cb;
	cb.advance_frame = advanceFrame;
	cb.load_game_state = loadGameState;
	cb.begin_game = beginGame;
	cb.save_game_state = saveGameState;
	cb.load_game_state = loadGameState;
	cb.log_game_state = logGameState;
	cb.free_buffer = freeBuffer;
	cb.on_event = onEvent;

	GGPOSession* ggpo;

	auto ret = ggpo_start_spectating(&ggpo, &cb, game, num_players, sizeof(uint64_t), localport, host_ip, host_port);

	if (GGPO_SUCCEEDED(ret)) {
		return ggpo;
	}
	else {
		return nullptr;
	}
}

extern "C" int UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UggSetDisconnectNotifyStart(GGPOSession * ggpo, int timeout)
{
	UggCallLog("UggSetDisconnectNotifyStart");
	return ggpo_set_disconnect_notify_start(ggpo, timeout);
}

extern "C" int UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UggSetDisconnectTimeout(GGPOSession * ggpo, int timeout)
{
	UggCallLog("UggSetDisconnectTimeout");
	return ggpo_set_disconnect_timeout(ggpo, timeout);
}

extern "C" int UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UggSynchronizeInput(GGPOSession * ggpo, uint64_t * inputs, int length, int& disconnect_flags)
{
	UggCallLog("UggSynchronizeInput");
	return ggpo_synchronize_input(ggpo, inputs, sizeof(uint64_t) * length, &disconnect_flags);
}

extern "C" int UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UggAddLocalInput(GGPOSession * ggpo, int local_player_handle, uint64_t input)
{
	UggCallLog("UggAddLocalInput");
	return ggpo_add_local_input(ggpo, local_player_handle, &input, sizeof(uint64_t));
}

extern "C" int UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UggCloseSession(GGPOSession * ggpo)
{
	UggCallLog("UggCloseSession");
	return ggpo_close_session(ggpo);
}

extern "C" int UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UggIdle(GGPOSession * ggpo, int timeout)
{
	UggCallLog("UggIdle");
	return ggpo_idle(ggpo, timeout);
}

extern "C" int UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UggAddPlayer(GGPOSession * ggpo,
	int player_type,
	int player_num,
	const char* player_ip_address,
	short player_port,
	int& phandle)
{
	UggCallLog("UggAddPlayer");
	GGPOPlayer player;
	player.size = sizeof(GGPOPlayer);
	player.type = (GGPOPlayerType)player_type;
	player.player_num = player_num;
	strcpy_s(player.u.remote.ip_address, player_ip_address);
	player.u.remote.port = player_port;
	return ggpo_add_player(ggpo, &player, &phandle);
}

extern "C" int UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UggDisconnectPlayer(GGPOSession * ggpo, int phandle)
{
	UggCallLog("UggDisconnectPlayer");
	return ggpo_disconnect_player(ggpo, phandle);
}

extern "C" int UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UggSetFrameDelay(GGPOSession * ggpo, int phandle, int frame_delay)
{
	UggCallLog("UggSetFrameDelay");
	return ggpo_set_frame_delay(ggpo, phandle, frame_delay);
}

extern "C" int UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UggAdvanceFrame(GGPOSession * ggpo)
{
	UggCallLog("UggAdvanceFrame");
	return ggpo_advance_frame(ggpo);
}

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UggLog(GGPOSession * ggpo, const char* v)
{
	UggCallLogv("UggLog %s", v);
	return ggpo_log(ggpo, v);
}

extern "C" int UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UggGetNetworkStats(GGPOSession * ggpo, int phandle,
	int& send_queue_len,
	int& recv_queue_len,
	int& ping,
	int& kbps_sent,
	int& local_frames_behind,
	int& remote_frames_behind)
{
	UggCallLogv("UggGetNetworkStats %i", phandle);
	GGPONetworkStats stats;
	auto result = ggpo_get_network_stats(ggpo, phandle, &stats);
	send_queue_len = stats.network.send_queue_len;
	recv_queue_len = stats.network.recv_queue_len;
	ping = stats.network.ping;
	kbps_sent = stats.network.kbps_sent;
	local_frames_behind = stats.timesync.local_frames_behind;
	remote_frames_behind = stats.timesync.remote_frames_behind;
	return result;
}
