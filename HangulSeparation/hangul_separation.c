#include <stdio.h>
#include <windows.h> 
#include <locale.h>

//�ʼ� 
static const wchar_t wcHead[] = { L'��', L'��', L'��', L'��',
L'��', L'��', L'��', L'��',
L'��', L'��', L'��', L'��',
L'��', L'��', L'��', L'��',
L'��', L'��', L'��' };

//�߼� 
static const wchar_t wcMid[] = { L'��', L'��', L'��', L'��',
L'��', L'��', L'��', L'��',
L'��', L'��', L'��', L'��',
L'��', L'��', L'��', L'��',
L'��', L'��', L'��', L'��', L'��' };

//���� 
static const wchar_t wcTail[] = { L' ', L'��', L'��', L'��',
L'��', L'��', L'��', L'��',
L'��', L'��', L'��', L'��',
L'��', L'��', L'��', L'��',
L'��', L'��', L'��', L'��',
L'��', L'��', L'��', L'��',
L'��', L'��', L'��', L'��' };

int BreakHangul(wchar_t *str, wchar_t *cho_buffer, wchar_t *buffer, UINT nSize)
{
	UINT pos = 0;
	UINT cho = 0;

	while (*str != '\0')
	{
		if (*str < 256)
		{
			if (pos + 2 >= nSize - 1)
				break;

			buffer[pos] = *str;
			++pos;
		}
		else
		{
			if (pos + 4 >= nSize - 1) 
				break;

			buffer[pos] = wcHead[(*str - 0xAC00) / (21 * 28)];
			cho_buffer[cho] = wcHead[(*str - 0xAC00) / (21 * 28)];
			buffer[pos + 1] = wcMid[(*str - 0xAC00) % (21 * 28) / 28];
			buffer[pos + 2] = wcTail[(*str - 0xAC00) % 28];

			pos += 3;
			++cho;
		}
		++str;
	}
	
	buffer[pos] = '\0';
	cho_buffer[cho] = '\0';

	return pos;
}

void MergeHangul(wchar_t *buffer, wchar_t *str, UINT pos) {
	int i, cho_i, jung_i, jong_i;
	UINT idx = 0;
	UINT cho = 0, jung = 0, jong = 0;

	for (i = 0; i < pos; i++) {
		if (isascii(buffer[i])) {	// �ƽ�Ű�ڵ��� ���
			str[idx++] = buffer[i];
		}
		else {  // �ѱ��� ���
			// �ʼ�
			for (cho_i = 0; cho_i < 19; cho_i++) {
				if (wcHead[cho_i] == buffer[i]) {
					cho = cho_i;
					break;
				}
			}
			// �߼�
			for (jung_i = 0; jung_i < 21; jung_i++) {
				if (wcMid[jung_i] == buffer[i + 1]) {
					jung = jung_i;
					break;
				}
			}
			// ����
			for (jong_i = 0; jong_i < 28; jong_i++) {
				if (wcTail[jong_i] == buffer[i + 2]) {
					jong = jong_i;
					break;
				}
			}

			str[idx++] = 0xAC00 + ((cho * 21) + jung) * 28 + jong;

			cho = 0;	jung = 0;	jong = 0;

			i += 2;
		}
	}
}

int main()
{
	wchar_t *str = L"�� a��b��c�� hangul�Դϴ�.";
	wchar_t buffer[4096];
	wchar_t cho_buffer[4096];
	wchar_t *str_buffer[4096];

	UINT pos;

	ZeroMemory(str_buffer, 4069);  // 0���� �ʱ�ȭ

	setlocale(LC_ALL, "Korean");

	pos = BreakHangul(str, cho_buffer, buffer, sizeof buffer);
	
	printf("1. �ʼ� : %S\n", cho_buffer);
	printf("2. �ʼ�/�߼�/���� : %S\n", buffer);

	MergeHangul(buffer, str_buffer, pos);

	printf("3. ���� : %S\n", str_buffer);

	return 0;
}