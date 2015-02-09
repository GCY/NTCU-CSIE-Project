#include <string>

int isbnDigitToInt(const char digit);

/**
 * 檢查 ISBN-10 或 ISBN-13 碼是否有效。
 * @return      true 表示有效
 * @param isbn  ISBN 字串，無連字號'-'。
 */
bool isIsbnValid(std::string isbn)
{
    if (isbn.length() < 10) {
        return false;
    } else if (isbn.length() < 13) {
        // ISBN-10
        int i, a = 0, b = 0;
        for (i = 0; i < 10; i++) {
            if (isbnDigitToInt(isbn.at(i)) == -1) {
                return false;
            }
            a += isbnDigitToInt(isbn.at(i));
            b += a;
        }
        return (b % 11 == 0);
    } else {
        // ISBN-13
        int i;
        int odd_digit_sum = 0, even_digit_sum = 0;
        for (i = 0; i<=12; i += 2) {
            odd_digit_sum += isbnDigitToInt(isbn.at(i));
            if (isbnDigitToInt(isbn.at(i)) == -1) {
                return false;
            }
        }
        for (i = 1; i<=11; i += 2) {
            even_digit_sum += isbnDigitToInt(isbn.at(i));
            if (isbnDigitToInt(isbn.at(i)) == -1) {
                return false;
            }
        }
        return ((odd_digit_sum+3*even_digit_sum)%10 == 0);
    }
}

/**
 * 把 ASCII 模式的 ISBN 位數轉換為 int 型態
 * @return       int 型態的位數，可爲 0..10; -1 表示 illegal.
 * @param digit  ISBN digit，可爲 '0'-'9', 'X', 'x'。
 */
int isbnDigitToInt(const char digit)
{
    if (digit == 'x' || digit == 'X'){
        return 10;
    } else if (digit >= '0' || digit <= '9') {
        return (digit-'0');
    } else {
        // illegal
        return -1;
    }

}
