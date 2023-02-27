#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LEN 100  // 定义最大字符串长度

#define bool int // 定义布尔类型
#define true 1 // 定义布尔真
#define false 0 // 定义布尔假

#define PRECISION 5 // 精度，即小数点后保留的位数

int decimals1 = 0;    // 小数位数1
int decimals2 = 0;    // 小数位数2

void multiply(char* num1, char* num2);  // 乘法函数声明
void subtract(char* num1, char* num2);  // 减法函数声明
void subtractHelping(char* num1, char* num2,int i, int j, int len1, int len2, bool isFlag);  // 减法辅助函数声明
void divide(char* num1, char* num2);  // 除法函数声明
void add(char* num1, char* num2, bool allNegative);  // 加法函数声明
bool whetherIsDigit(char* num, int type);  // 判断是否是数字函数声明
bool isNegative(char* num);  // 判断是否是负数函数声明
int realLen(char* num);  // 获取真实长度函数声明
bool isSmaller(char* num1, char* num2); // 判断num1是否小于num2函数声明
bool isSmaller2(char* num1, char* num2, int curIndex); // 判断num1是否小于num2函数声明（特供版）



void multiply(char* num1, char* num2) {
    //如果两个都是正数，或都是负数，则不用加负号。如果一正一负，则加负号
    bool addFu = false;
    if ((isNegative(num1) && !isNegative(num2)) || (!isNegative(num1) && isNegative(num2))) {
        addFu = true;
    }

    //把负号去掉
    if (isNegative(num1)) {
        num1++;
    }
    if (isNegative(num2)) {
        num2++;
    }
    int len1 = strlen(num1);
    int len2 = strlen(num2); 
    int realLen1 = realLen(num1);
    int realLen2 = realLen(num2);

    int result[MAX_LEN * 2] = {0};  // 用于保存计算结果，初始化为0
    int carry = 0;  // 进位标志，初始化为0
    int last = 0;  // 最后一个非零位的位置

    int i, j, k;
    // 从低位到高位逐位计算
    for (i = len1 - 1; i >= len1 - realLen1; i--) {
        for (j = len2 - 1; j >= len2 - realLen2; j--) {
            k = len1 + len2 - 2 - i - j;  // 计算结果数组中的位置
            result[k] += (num1[i] - '0') * (num2[j] - '0');  // 相乘并加到对应位置
            result[k + 1] += result[k] / 10;  // 处理进位
            result[k] %= 10;
        }
    }

    // 找到最后一个非零位
    for (k = len1 + len2 - 1; k >= 0; k--) {
        if (result[k] != 0) {
            last = k;
            break;
        }
    }
    // 倒序输出计算结果
    if(addFu) {
        printf("-");
    }
    for (i = last; i >= 0; i--) {
        
        if(i == decimals1 + decimals2 - 1) {
            printf(".");
        }
        printf("%d", result[i]);
    }
    printf("\n");
}

void add(char* num1, char* num2, bool allNegative) {
    //去掉负号
    if(isNegative(num1)){
        num1++;
    }
    if(isNegative(num2)){
        num2++;
    }
    
    //获取两个数的长度
    int len1 = strlen(num1);
    int len2 = strlen(num2);

    //如果decimals1和decimals2不相等，则往小的那个数的末尾补0
    if (decimals1 > decimals2) {
        for (int i = 0; i < decimals1 - decimals2; i++) {
            num2[len2++] = '0';
        }
    } else if (decimals1 < decimals2) {
        for (int i = 0; i < decimals2 - decimals1; i++) {
            num1[len1++] = '0';
        }
    }

    //手动添加终止符
    num1[len1] = '\0';
    num2[len2] = '\0';
    
    //定义最大长度为，MAX_LEN + 1;
    int maxLen = MAX_LEN + 1;
    int result[MAX_LEN + 1] = {0}; // 用于存储计算结果，需要多分配一位来处理进位(动态数组)
    memset(result, 0, sizeof(result)); // 初始化为0

    // 从低位到高位逐位相加
    for (int i = 0; i < maxLen - 1; i++) {
        int a = i < len1 ? (num1[len1 - 1 - i] - '0') : 0;
        int b = i < len2 ? (num2[len2 - 1 - i] - '0') : 0;
        result[maxLen - i - 1] += a + b;
    }

    // 处理进位
    for (int i = maxLen - 1; i > 0; i--) {
        if (result[i] >= 10) {
            result[i - 1] += result[i] / 10;
            result[i] %= 10;
        }
    }

    //找到第一个非零位
    int last = 0;
    for (int i = 0; i < maxLen; i++) {
        if (result[i] != 0) {
            last = i;
            break;
        }
    }
    
    // 从最后一个非零位开始，输出结果
    //如果两个数都是负数，则输出负号
    if (allNegative) {
        printf("-");
    }
    int biggerDecimal = decimals1 > decimals2 ? decimals1 : decimals2;
    for (int i = last; i < maxLen; i++) {
        //打印小数点
        if(i == maxLen - biggerDecimal) {
            printf(".");
        }
        printf("%d", result[i]);
    }

    printf("\n");
}

void subtract(char* num1, char* num2) {
    //去掉负号
    if(isNegative(num1)){
        num1++;
    }
    if(isNegative(num2)){
        num2++;
    }

    //获取两个数的长度
    int len1 = strlen(num1);
    int len2 = strlen(num2);
    //如果decimals1和decimals2不相等，则往小的那个数的末尾补0
    if (decimals1 > decimals2) {
        for (int i = 0; i < decimals1 - decimals2; i++) {
            num2[len2++] = '0';
            
        }
    } else if (decimals1 < decimals2) {
        for (int i = 0; i < decimals2 - decimals1; i++) {
            num1[len1++] = '0';
        }
    }

    //手动添加终止符
    num1[len1] = '\0';
    num2[len2] = '\0';

    //长度
    int i = len1 - 1, j = len2 - 1;

    //比较num1和num2的大小，如果num1小于num2，则交换两个数
    //并设置一个flag，如果真的交换了两个数，则最后输出结果时要加上负号
    bool smallerFlag = isSmaller(num1, num2);

    if(smallerFlag){
       subtractHelping(num2, num1, j, i, len2, len1, smallerFlag);
    }else{
       subtractHelping(num1, num2, i, j, len1, len2, smallerFlag);
    }

}

void subtractHelping(char* num1, char* num2, int i, int j, int len1, int len2, bool smallerFlag) {
        while (i >= 0 && j >= 0) {
        if (num1[i] >= num2[j]) {
            num1[i] = num1[i] - num2[j] + '0';
        } else {
            num1[i] = num1[i] + 10 - num2[j] + '0';
            int k = i - 1;
            while (num1[k] == '0') {
                num1[k] = '9';
                k--;
            }
            num1[k] = num1[k] - 1;
        }
        i--;
        j--;
    }
    while (num1[0] == '0' && len1 > 1) {
        for (int k = 0; k < len1 - 1; k++) {
            num1[k] = num1[k + 1];
        }
        num1[len1 - 1] = '\0';
        len1--;
    }

    //打印结果
    int biggerDecimal = decimals1 > decimals2 ? decimals1 : decimals2;
    int maxLen = len1 > len2 ? len1 : len2;

    if (smallerFlag) {
        printf("-");
    }
    for(int i = 0; i < len1; i++) {
        //打印小数点
        if(i == maxLen - biggerDecimal) {
            printf(".");
        }
        printf("%c", num1[i]);
    }
}


void divide(char* num1, char* num2){
    //如果两个都是正数，或都是负数，则不用加负号。如果一正一负，则加负号
    bool addFu = false;
    if ((isNegative(num1) && !isNegative(num2)) || (!isNegative(num1) && isNegative(num2))) {
        addFu = true;
    }

    //把负号去掉
    if (isNegative(num1)) {
        num1++;
    }
    if (isNegative(num2)) {
        num2++;
    }

    int len1 = strlen(num1);
    int realLen1 = len1;
    int len2 = realLen(num2);
    //如果num2为0，则报错
    if(len2 == 0) {
        printf("NAN (A number cannot be divied by zero.)");
        return;
    }
    //如果num1比num2小，则设置一个flag，方便最后打印时，先打印0
    bool smallerFlag = isSmaller(num1, num2);
    //定义了保留PRECISION位小数。所以，在num1后面补上PRECISION位0
    for(int i = 0; i < PRECISION; i++) {
        num1[len1] = '0';
        len1++;
    }

    //开始除法。从num1的第len2位开始。
    //取num1的前len2位的数字为num，若小于num2，则i++，并让num的末尾添加上新增的num1的数字。
    //若num大于num2，则开始除法。每次除法，num减去num2，同时计数，计数的次数就是商。
    //最后，将商添加到结果中。
    char* result = (char*)malloc(sizeof(char) * (MAX_LEN)); //结果
    //将result填满'0'
    for(int i = 0; i < MAX_LEN; i++) {
        result[i] = '0';
    }
    
    int curIndex = len2 - 1; //当前减的num，最末位是num1的第curIndex位
    int realIndex = curIndex;
    
    while(realIndex < realLen1 + PRECISION) {
        
        //如果num小于num2，则curIndex++
        if(isSmaller2(num1, num2, curIndex)) {
            curIndex++;
            realIndex++;
        }else{
            //若num大于num2，则开始除法。每次除法，num减去num2，同时计数，计数的次数就是商。
            int count = 0;
            while(!isSmaller2(num1, num2,curIndex)) { 
                //如果num1的第一位是0，则去掉
                while(num1[0] == '0') {
                    num1++;
                    len1--;
                    curIndex--;
                }
                //再次检查
                if(isSmaller2(num1, num2,curIndex)){
                    // printf("check fail!\n");
                    break;
                }
                int j = len2 - 1;
                //用num1的第0位到第curIndex位的数字，减去num2(注意：num2的长度可能小于num1的长度)（不调用subtract方法）
                for(int i = curIndex; j >= 0; i--, j--){
                    
                    if(num1[i] >= num2[j]) {
                        num1[i] = num1[i] - num2[j] + '0';
                    }else{
                        num1[i] = num1[i] + 10 - num2[j] + '0';
                        int k = i - 1;
                        while(num1[k] == '0') {
                            num1[k] = '9';
                            k--;
                        }
                        num1[k] = num1[k] - 1;
                    }
                }
                count++;
            }
            // printf("%d\n",count);
            result[realIndex - len2 + 1] = count + '0';
        }
    }
    result[realLen1 + PRECISION - len2 + 1] = '\0';
    // printf("length is : %d\n",realLen1 + PRECISION - len2 + 1);
    // printf("length is : %d\n",strlen(result));
    //打印结果(注意打印小数点)（还有负号）
    if(addFu) {
        printf("-");
    }
    if(smallerFlag){
        for(int i = 0; i < PRECISION + 1; i++){
            printf("%c",result[i]);
            if(i == 0) {
                printf(".");
            }
        }
    }else{
        //如果第一位是零，则跳过
        int last = 0;
        while(result[last] == '0') {
            last++;
        }
        int count = 0;
        for(int i = last; i < strlen(result); i++) {
            if(i == strlen(result) - PRECISION - (decimals1 - decimals2)) {
                //如果是第一位，则打印0
                if(i == last) {
                    printf("0");
                }
                printf(".");
            }
            //输出五位就退出
            if(i >= strlen(result) - PRECISION - (decimals1 - decimals2)){
                count++;
                if(count == PRECISION + 1) {
                    break;
                }
            }
            printf("%c", result[i]);
        }
        //如果位数不够（count没到6），则补0
        while(count < PRECISION) {
            printf("0");
            count++;
        }
    }
    
}

bool isSmaller(char* num1, char* num2){
    int len1 = strlen(num1) - decimals1; //整数长度
    int len2 = strlen(num2) - decimals2; //整数长度
    if (len1 < len2) {
        return true;
    } else if (len1 == len2) {
        for (int i = 0; i < len1; i++) {
            if(num1[i] > num2[i]) {
                return false;
            }
            if (num1[i] < num2[i]) {
                return true;
            }
        }
        //再验证小数的
        for(int i = len1 + 1; i < strlen(num1); i++) {
            if(num1[i] > num2[i]) {
                return false;
            }
            if (num1[i] < num2[i]) {
                return true;
            }
        }
    }
    return false;
}

bool isSmaller2(char* num1, char* num2, int curIndex){
    int len1 = curIndex + 1;
    int len2 = strlen(num2);
    if (len1 < len2) {
        return true;
    } else if (len1 == len2) {
        for (int i = 0; i < len1; i++) {
            if(num1[i] > num2[i]) {
                return false;
            }
            else if (num1[i] < num2[i]) {
                return true;
            }
        }
    }
    return false;
}

bool whetherIsDigit(char* num, int type) {
    //如果第一个字符是负号，则从第二个字符开始判断
    int i = 0;
    if (num[0] == '-') {
        i = 1;
    }
    //如果其中有小数点，则将后面的数字内容，集体前移一位。同时计数，小数点后有多少位
    //如果有多个小数点，则报错
    int dotCount = 0;
    for (; i < strlen(num); i++) {
        if (num[i] == '.') {
            dotCount++;
            if (dotCount > 1) {
                return false;
            }
            for (int j = i; j < strlen(num) - 1; j++) {
                num[j] = num[j + 1];
                if(type == 1){
                    decimals1++;
                }else{
                    decimals2++;
                }
            }
            num[strlen(num) - 1] = '\0';
        }else if (num[i] < '0' || num[i] > '9'){
            return false;
        }
    }
    return true;
}

bool isNegative(char* num) {
    if (num[0] == '-') {
        return true;
    }
    return false;
}

//计算实际长度，去掉前面的0
int realLen(char* num) {
    int i = 0;
    if (num[0] == '-') {
        i = 1;
    }
    for (; i < strlen(num); i++) {
        if (num[i] != '0') {
            return strlen(num) - i;
        }
    }
    return 0;
}

//等待完成：科学计数法的处理
int main(int argc, char *argv[]) {
    char a[MAX_LEN], b[MAX_LEN], operate;
    bool canFlag = false;
    bool noDigitFlag = false;
    while(!canFlag){
        if (argc != 4) {
            if(noDigitFlag){
                printf("The input cannot be interpret as numbers!\n");
            }else{
                printf("You have to enter the correct number of arguments!\n");
            }
    
            while (1) {
                char input[MAX_LEN*3];
                printf("Enter the calculation (e.g. 1.23 + 4.56): ");
                fgets(input, MAX_LEN*3, stdin);
                if (sscanf(input, "%s %c %s", a, &operate, b) == 3) {
                    break;
                }
                printf("Invalid input, please try again.\n");
            }
        }else{
            // 读入数据
            operate = argv[2][0];
            strcpy(a,argv[1]);
            strcpy(b,argv[3]);
        }
        
        //将这个算式打印出来
        printf("%s %c %s = ", a, operate, b);
        //判断是否是数字
        if (whetherIsDigit(a,1) && whetherIsDigit(b,2)) {
            canFlag = true;
            if(operate == '+'){
                //如果两个数字都是正数或者都是负数，则直接相加。但若是负数，则在最后输出结果时加上负号。
                if(isNegative(a) && isNegative(b)){
                    add(a, b, true);
                }else if(!isNegative(a) && !isNegative(b)){
                    add(a, b, false);
                }else if(isNegative(a) && !isNegative(b)){
                    //将decimals交换一下
                    int temp = decimals1;
                    decimals1 = decimals2;
                    decimals2 = temp;
                    subtract(b, a);
                }else{
                    subtract(a, b);
                }
            }else if(operate == '-'){
                //如果被减数是负数，减数是正数，则直接相加。但若是负数，则在最后输出结果时加上负号。
                if(isNegative(a) && !isNegative(b)){
                    add(a, b, true);
                }else if(!isNegative(a) && isNegative(b)){
                    add(a, b, false);
                }else if(isNegative(a) && isNegative(b)){
                    //将decimals交换一下
                    int temp = decimals1;
                    decimals1 = decimals2;
                    decimals2 = temp;
                    subtract(b, a);
                }else{
                    subtract(a, b);
                }
            }else if(operate == '^'){
                //负号已经在方法内讨论了
                multiply(a, b);
            }else if(operate == '/'){
                divide(a, b);
            }else{
                printf("输入错误");
            }
        } else {
            noDigitFlag = true;
            argc = 3;
        }
    }
    return 0;
}
