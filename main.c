#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 1024
#define CSV_DELIMETER ','

typedef struct
{
    char *loginId;
    char *password;
    char *name;
    char *kanaName;
} UserInfo;

typedef struct
{
    uint32_t size;
    UserInfo **users;
} UserList;

void freeUserInfo(UserInfo *user)
{
    if (user->loginId != NULL)
    {
        free(user->loginId);
    }
    if (user->password != NULL)
    {
        free(user->password);
    }
    if (user->name != NULL)
    {
        free(user->name);
    }
    if (user->kanaName != NULL)
    {
        free(user->kanaName);
    }
}

void addUser(UserList *list, UserInfo *user)
{
    list->size += 1;
    if (list->size == 1)
    {
        list->users = (UserInfo **)malloc(list->size * sizeof(UserInfo *));
    }
    else
    {
        list->users = (UserInfo **)realloc(list->users, list->size * sizeof(UserInfo *));
    }
    list->users[list->size - 1] = user;
}

void printUserInfo(UserInfo user) {
    printf("%s, %s, %s\n", user.loginId, user.name, user.kanaName);
}

void printUsers(UserList list)
{
    for (int i = 0; i < list.size; i++)
    {
        printUserInfo(*(list.users[i]));
    }
}

void freeList(UserList *list)
{
    if (list->size == 0)
    {
        return;
    }
    for (int i = 0; i < list->size; i++)
    {
        freeUserInfo(list->users[i]);
        free(list->users[i]);
    }
    free(list->users);
    list->size = 0;
}

char *getCsvField(char *line, int *index)
{
    char *buf = (char *)malloc(sizeof(char));
    int i = *index;
    int j = 0;

    while (line[i] != CSV_DELIMETER && line[i] != '\0' && line[i] != '\n')
    {
        buf = (char *)realloc(buf, (j + 2) * sizeof(char));
        buf[j] = line[i];
        i += 1;
        j += 1;
    }
    buf[j] = '\0';
    *index = i;

    return buf;
}

UserInfo *createUser(char *line)
{
    UserInfo *user = (UserInfo *)malloc(sizeof(UserInfo));
    int i = 0;

    user->loginId = getCsvField(line, &i);
    i += 1;
    user->password = getCsvField(line, &i);
    i += 1;
    user->name = getCsvField(line, &i);
    i += 1;
    user->kanaName = getCsvField(line, &i);

    return user;
}

int main(void)
{
    printf("Hello world\n");

    UserList list = {0};

    // Read file
    char filename[] = "users.csv";
    char delimeter = ',';
    FILE *fp;
    char buf[BUF_SIZE], *line;
    printf("Reading %s\n", filename);
    if ((fp = fopen(filename, "r")) == NULL)
    {
        printf("File open error\n");
        return 1;
    }
    while ((line = fgets(buf, BUF_SIZE, fp)) != NULL)
    {
        UserInfo *user = createUser(line);
        addUser(&list, user);
    }
    fclose(fp);

    printf("LoginID, Name, Kana Name\n");
    printf("------------------------\n");
    printUsers(list);
    freeList(&list);
    return 0;
}
