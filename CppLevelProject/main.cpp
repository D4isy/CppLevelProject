
#include <conio.h>
#include <Windows.h>

#include <iostream>

using namespace std;

#define NAMESIZE 64
#define ACCOUNTSIZE 256

enum ACC_TYPE {
	ACC_EXIT,
	ACC_CREATE,
	ACC_DEPOSIT,
	ACC_WITHDRAW,
	ACC_PRINTALL,
};

typedef struct _typeAccount {
	int id;
	char name[NAMESIZE];
	unsigned int money;
} ACCOUNT, *PACCOUNT;

void Pause();

bool Init(int count = ACCOUNTSIZE);
void Release();

bool AccountInsert(const ACCOUNT& account);
int originalAccountId(int id);

int AccountMenu();
bool AccountCreate();
bool AccountDeposit();
bool AccountWithdraw();
void AccountPrintAll();

bool bRun = true;
PACCOUNT tAccount;
int iAccountCount;
int iAccountMaxCount;

int main(void) {
	
	Init();

	int in;
	while (bRun) {
		in = AccountMenu();
		switch (in) {
		case ACC_EXIT:
			bRun = false;
			break;

		case ACC_CREATE:
			if (!AccountCreate()) {
				cout << "생성 실패" << endl;
			}
			else {
				cout << "생성 완료" << endl;
			}
			Pause();
			break;

		case ACC_DEPOSIT:
			if (!AccountDeposit()) {
				cout << "입금 실패" << endl;
			}
			else {
				cout << "입금 완료" << endl;
			}
			Pause();
			break;

		case ACC_WITHDRAW:
			if (!AccountWithdraw()) {
				cout << "출금 실패" << endl;
			}
			else {
				cout << "출금 완료" << endl;
			}
			Pause();
			break;

		case ACC_PRINTALL:
			AccountPrintAll();
			Pause();
			break;
		}
	}

	Release();
	return 0;
}

void Pause()
{
	int etc = _getch();
	if (etc == 0x00 || etc == 0xE0) {
		_getch();
	}
}

bool Init(int count)
{
	iAccountMaxCount = count;
	iAccountCount = 0;
	tAccount = (PACCOUNT)malloc(sizeof(ACCOUNT) * count);
	memset(tAccount, 0, sizeof(ACCOUNT) * count);
	return true;
}

void Release()
{
	free(tAccount);
}

bool AccountInsert(const ACCOUNT & account)
{
	if (iAccountCount >= iAccountMaxCount) {
		return false;
	}

	tAccount[iAccountCount].id = account.id;
	strcpy_s<NAMESIZE>(tAccount[iAccountCount].name, account.name);
	tAccount[iAccountCount].money = account.money;
	iAccountCount++;
	return true;
}

int originalAccountId(int id)
{
	PACCOUNT iter = tAccount;
	for (int i = 0; i < iAccountCount; i++) {
		if (iter[i].id == id) {
			return i;
		}
	}
	return -1;
}

int AccountMenu()
{
	system("cls");
	cout << "----- Menu -----" << endl;
	cout << "0. 종    료" << endl;
	cout << "1. 계좌개설" << endl;
	cout << "2. 입    금" << endl;
	cout << "3. 출    금" << endl;
	cout << "4. 계좌정보(전체)" << endl;
	cout << "> ";

	int input;
	cin >> input;

	return input;
}

bool AccountCreate()
{
	system("cls");
	ACCOUNT newAccount;

	cout << "[계좌개설]" << endl;
	cout << "계좌ID: ";
	cin >> newAccount.id;
	if (originalAccountId(newAccount.id) != -1) {
		return false;
	}

	cout << "이  름: ";
	cin >> newAccount.name;

	cout << "입금액: ";
	cin >> newAccount.money;
	if (newAccount.money < 0) {
		return false;
	}

	if (!AccountInsert(newAccount)) {
		return false;
	}
	return true;
}

bool AccountDeposit()
{
	system("cls");
	ACCOUNT newAccount;

	cout << "[입    금]" << endl;
	cout << "계좌ID: ";
	cin >> newAccount.id;

	int idx;
	if ((idx = originalAccountId(newAccount.id)) == -1) {
		return false;
	}

	cout << "입금액: ";
	cin >> newAccount.money;
	if (newAccount.money < 0) {
		return false;
	}

	tAccount[idx].money += newAccount.money;
	cout << "잔  액: " << tAccount[idx].money << endl;
	return true;
}

bool AccountWithdraw()
{
	system("cls");
	ACCOUNT newAccount;

	cout << "[출    금]" << endl;
	cout << "계좌ID: ";
	cin >> newAccount.id;

	int idx;
	if ((idx = originalAccountId(newAccount.id)) == -1) {
		return false;
	}

	cout << "현재잔액: " << tAccount[idx].money << endl;
	cout << "출금액: ";
	cin >> newAccount.money;
	if (newAccount.money < 0) {
		return false;
	}
	else if (tAccount[idx].money < newAccount.money) {
		return false;
	}

	tAccount[idx].money -= newAccount.money;
	cout << "잔  액: " << tAccount[idx].money << endl;
	return true;
}

void AccountPrintAll()
{
	PACCOUNT iter = tAccount;
	cout << "계좌  ID\t이    름\t잔    액" << endl;
	for (int i = 0; i < iAccountCount; i++) {
		cout.width(8);
		cout.fill('0');
		cout << iter[i].id << "\t";

		cout.flags(ios::left);
		cout.fill(' ');
		cout.width(8);
		cout << iter[i].name << "\t";

		cout << iter[i].money << endl;
		cout.flags(ios::right);
	}
}
