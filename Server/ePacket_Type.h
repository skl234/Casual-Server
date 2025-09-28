#pragma once
#define HEADER_SIZE 4

enum class ePacket_Type : WORD
{
	Begin = 0,

	//Server_to_Client,
	S_TO_C_RESULT_ENTER_SERVER,
	S_TO_C_RESULT_ENTER_LOBBY,
	S_TO_C_USER_LIST,
	S_TO_C_ROOM_LIST,
	S_TO_C_LOBBY_CHAT,
	S_TO_C_RESULT_CREATE_ROOM,
	S_TO_C_RESULT_ENTER_ROOM,
	S_TO_C_ROOMINFO,
	S_TO_C_ROOM_CHAT,
	S_TO_C_ENTER_USER_INROOM,
	S_TO_C_LEAVE_USER_INROOM,

	//Client_to_Server
	C_TO_S_REQUEST_ENTER_LOBBY,
	C_TO_S_REQUEST_USER_LIST,
	C_TO_S_REQUEST_ROOM_LIST,
	C_TO_S_LOBBY_CHAT,
	C_TO_S_CREATE_ROOM,
	C_TO_S_ENTER_ROOM,
	C_TO_S_REQUEST_ROOMINFO,
	C_TO_S_LEAVE_ROOM,
	C_TO_S_ROOM_CHAT,

	//4 HandShake
	C_TO_S_REQUEST_CLOSE_CLIENT,
	S_TO_C_ACK_CLOSE_CLIENT,
	S_TO_C_READY_CLOSE_CLINET,
	C_TO_S_CLOSE_CLIENT,

	End


	// Ŭ���̾�Ʈ���� ������
	// - ������ ��û
	// - ������ ����
	// 
	// �������� Ŭ���̾�Ʈ��
	// - ��û���� ������ ����
	// - �Ϲ������� ������ ����
	// 
	// ���� �ΰ����� �ִ´�...
	// �ϴ� ���ΰ����� UDP�� ������ �����̴� �н�
	// �׷��ٸ�?
	// 
	// 
	// 
	// 
};