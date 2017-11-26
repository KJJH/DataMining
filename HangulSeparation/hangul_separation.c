#include <stdio.h>
#include <windows.h> 
#include <locale.h>

//초성 
static const wchar_t wcHead[] = { L'ㄱ', L'ㄲ', L'ㄴ', L'ㄷ',
L'ㄸ', L'ㄹ', L'ㅁ', L'ㅂ',
L'ㅃ', L'ㅅ', L'ㅆ', L'ㅇ',
L'ㅈ', L'ㅉ', L'ㅊ', L'ㅋ',
L'ㅌ', L'ㅍ', L'ㅎ' };

//중성 
static const wchar_t wcMid[] = { L'ㅏ', L'ㅐ', L'ㅑ', L'ㅒ',
L'ㅓ', L'ㅔ', L'ㅕ', L'ㅖ',
L'ㅗ', L'ㅘ', L'ㅙ', L'ㅚ',
L'ㅛ', L'ㅜ', L'ㅝ', L'ㅞ',
L'ㅟ', L'ㅠ', L'ㅡ', L'ㅢ', L'ㅣ' };

//종성 
static const wchar_t wcTail[] = { L' ', L'ㄱ', L'ㄲ', L'ㄳ',
L'ㄴ', L'ㄵ', L'ㄶ', L'ㄷ',
L'ㄹ', L'ㄺ', L'ㄻ', L'ㄼ',
L'ㄽ', L'ㄾ', L'ㄿ', L'ㅀ',
L'ㅁ', L'ㅂ', L'ㅄ', L'ㅅ',
L'ㅆ', L'ㅇ', L'ㅈ', L'ㅊ',
L'ㅋ', L'ㅌ', L'ㅍ', L'ㅎ' };

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
		if (isascii(buffer[i])) {	// 아스키코드일 경우
			str[idx++] = buffer[i];
		}
		else {  // 한글일 경우
			// 초성
			for (cho_i = 0; cho_i < 19; cho_i++) {
				if (wcHead[cho_i] == buffer[i]) {
					cho = cho_i;
					break;
				}
			}
			// 중성
			for (jung_i = 0; jung_i < 21; jung_i++) {
				if (wcMid[jung_i] == buffer[i + 1]) {
					jung = jung_i;
					break;
				}
			}
			// 종성
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
	wchar_t *str = L"이 a문b장c은 hangul입니다.";
	wchar_t buffer[4096];
	wchar_t cho_buffer[4096];
	wchar_t *str_buffer[4096];

	UINT pos;

	ZeroMemory(str_buffer, 4069);  // 0으로 초기화

	setlocale(LC_ALL, "Korean");

	pos = BreakHangul(str, cho_buffer, buffer, sizeof buffer);
	
	printf("1. 초성 : %S\n", cho_buffer);
	printf("2. 초성/중성/종성 : %S\n", buffer);

	MergeHangul(buffer, str_buffer, pos);

	printf("3. 문장 : %S\n", str_buffer);

	return 0;
}