#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_TEXT 512

/**
 * @author Tinsae Debele[101034827]    Jorge Fernandez [101047124]
 * 
 */
/**
 *The struct is use to pass the data of the information  
 * 
 */

struct my_msg_st
{
    long int my_msg_type;     // long int for the message type
    char some_text[MAX_TEXT]; // for infomrtion passogn
};
/**
 * This struct is used to pass command from the client to server 
 * 
 */

struct my_cmd
{
    long int my_sag;    //command type
    char cmd[MAX_TEXT]; //care the command
};
/**
 * The struct is used to reciving the result from the server 
 */
struct resultCol
{
    long int reuslt_msg_type;
    char result[MAX_TEXT];
};
/**
 * The four data for each employee 
 * 
 */
struct empInfo
{
    long int my_msg_type;
    char empName[MAX_TEXT];        // employee's Name
    char empDep[MAX_TEXT];         // Depatment of the employ
    char employeeNumber[MAX_TEXT]; // employee Number
    char salary[MAX_TEXT];         // employee salary
};

/**
 * This Fuction is used to insert an employee to the system with it correspoing infomriont 
 * 
 * 
 * 
 */
void Insert(char empName[MAX_TEXT], char empDepar[MAX_TEXT], char employNum[MAX_TEXT], char empSalary[MAX_TEXT])
{

    int running = 1;     // used for while loop for sending and resiving
    struct empInfo data; // sturc type of empInfo used to create employee infomtion
    struct my_msg_st some_data;
    int msgid;                   // declearing our first message queue
    char buffer[BUFSIZ] = "end"; // char for ending the loop or the programm

    //-------------------------------------------------
    int msgid1;                      // declearing our second message queue for passing command
    struct my_cmd comand;            // struct for our command info
    char buffer2[BUFSIZ] = "Insert"; // our command

    msgid1 = msgget((key_t)4231, 0666 | IPC_CREAT); // create the first message queue for looping
    if (msgid1 == -1)
    {
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    /**---------------------------------------------------*/

    msgid = msgget((key_t)1234, 0666 | IPC_CREAT); // create the first message queue for looping
    // Copying the infomtiong to corresponding struct
    strcpy(data.empName, empName);
    strcpy(data.empDep, empDepar);
    strcpy(data.employeeNumber, employNum);
    strcpy(data.salary, empSalary);
    if (msgid == -1)
    {
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    int cnt = 0;
    while (running)
    {
        //--------------------------------------------------------------
        comand.my_sag = 1;
        strcpy(comand.cmd, buffer2);

        if (msgsnd(msgid1, (void *)&comand, MAX_TEXT, 0) == -1)
        {
            fprintf(stderr, "msgsnd failed.\n");
            exit(EXIT_FAILURE);
        }
        //--------------------------------------------------------------
        // Send each informtion
        some_data.my_msg_type = 1;
        switch (cnt)
        {
        case 0:
            strcpy(some_data.some_text, data.empName);
            if (msgsnd(msgid, (void *)&some_data, MAX_TEXT, 0) == -1)
            {
                fprintf(stderr, "msgsnd failed\n");
                exit(EXIT_FAILURE);
            }

            break;
        case 1:
            strcpy(some_data.some_text, data.empDep);

            if (msgsnd(msgid, (void *)&some_data, MAX_TEXT, 0) == -1)
            {
                fprintf(stderr, "msgsnd failed\n");
                exit(EXIT_FAILURE);
            }

            break;
        case 2:
            strcpy(some_data.some_text, data.employeeNumber);

            if (msgsnd(msgid, (void *)&some_data, MAX_TEXT, 0) == -1)
            {
                fprintf(stderr, "msgsnd failed\n");
                exit(EXIT_FAILURE);
            }

            break;
        case 3:
            strcpy(some_data.some_text, data.salary);

            if (msgsnd(msgid, (void *)&some_data, MAX_TEXT, 0) == -1)
            {
                fprintf(stderr, "msgsnd failed\n");
                exit(EXIT_FAILURE);
            }

            running = 0;
            break;
        }
        cnt++;
    }
}
/**
*in response to this command Record-Keeper must return the
*name of the employee with the employee number passed as an argument for this command. 
*/
//***********************Check Name**************************************//
void Check_name(char empNumber[MAX_TEXT])
{
    long int result_to_receive = 0;
    int running = 1;
    struct empInfo data;
    struct my_msg_st some_data;
    int msgid;
    //**************************************************************************
    struct resultCol resultPass;
    int client_id;
    client_id = msgget((key_t)2345, 0666 | IPC_CREAT);

    if (client_id == -1)
    {
        fprintf(stderr, "msgget (client_id) failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    //**************************************************************************

    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    if (msgid == -1)
    {
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }
    strcpy(some_data.some_text, empNumber);

    //*********************************************************************************
    int msgid1;
    struct my_cmd comand;
    char bufferCMD[MAX_TEXT] = "CheckName";

    msgid1 = msgget((key_t)4231, 0666 | IPC_CREAT);
    if (msgid1 == -1)
    {
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }
    while (running)
    {
        resultPass.reuslt_msg_type = getpid();
        comand.my_sag = 1;
        strcpy(comand.cmd, bufferCMD);
        printf("Command sent : %s\n", comand.cmd);
        if (msgsnd(msgid1, (void *)&comand, MAX_TEXT, 0) == -1)
        {
            fprintf(stderr, "msgsnd failed.\n");
            exit(EXIT_FAILURE);
        }

        //********************************************
        some_data.my_msg_type = 1;
        if (msgsnd(msgid, (void *)&some_data, MAX_TEXT, 0) == -1)
        {
            fprintf(stderr, "msgsnd failed\n");
            exit(EXIT_FAILURE);
        }

        int listening = 1;

        while (listening)
        {

            if (msgrcv(client_id, (void *)&resultPass, BUFSIZ, result_to_receive, 0) == -1)
            {
                fprintf(stderr, "msgrcv failed with error: %d\n", errno);
                exit(EXIT_FAILURE);
            }
            printf("\n****************Result************\n\n");
            printf("The Name of the employee is : %s\n\n", resultPass.result);
            printf("\n**********************************\n");
            listening = 0;
        }

        running = 0;
    }

    if (msgctl(client_id, IPC_RMID, 0) == -1)
    {
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }
}

/**
*in response to this command Record-Keeper must return the department name for the employee 
*with the employee number passed as an argument for this command.
*/

void Check_department(char empNumber[MAX_TEXT])
{
    long int result_to_receive = 0;
    int running = 1;
    struct empInfo data;
    struct my_msg_st some_data;
    int msgid;
    //**************************************************************************
    struct resultCol resultPass;
    int client_id;
    client_id = msgget((key_t)2345, 0666 | IPC_CREAT);

    if (client_id == -1)
    {
        fprintf(stderr, "msgget (client_id) failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    //**************************************************************************

    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    if (msgid == -1)
    {
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }
    strcpy(some_data.some_text, empNumber);

    //*********************************************************************************
    int msgid1;
    struct my_cmd comand;
    char bufferCMD[MAX_TEXT] = "CheckDep";

    msgid1 = msgget((key_t)4231, 0666 | IPC_CREAT);
    if (msgid1 == -1)
    {
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }
    while (running)
    {
        resultPass.reuslt_msg_type = getpid();
        comand.my_sag = 1;
        strcpy(comand.cmd, bufferCMD);
        printf("Command sent : %s\n", comand.cmd);
        if (msgsnd(msgid1, (void *)&comand, MAX_TEXT, 0) == -1)
        {
            fprintf(stderr, "msgsnd failed.\n");
            exit(EXIT_FAILURE);
        }

        //********************************************
        some_data.my_msg_type = 1;
        if (msgsnd(msgid, (void *)&some_data, MAX_TEXT, 0) == -1)
        {
            fprintf(stderr, "msgsnd failed\n");
            exit(EXIT_FAILURE);
        }

        int listening = 1;

        while (listening)
        {

            if (msgrcv(client_id, (void *)&resultPass, BUFSIZ, result_to_receive, 0) == -1)
            {
                fprintf(stderr, "msgrcv failed with error: %d\n", errno);
                exit(EXIT_FAILURE);
            }
            printf("\n****************Result************\n\n");
            printf("Department of the employee is : %s\n\n", resultPass.result); //message Out put form the server
            printf("\n**********************************\n");
            listening = 0;
        }

        running = 0;
    }

    if (msgctl(client_id, IPC_RMID, 0) == -1)
    {
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }
}
/**
 * : in response to this command Record-Keeper must return the
 *salary of the employee with the employee number passed as an argument for this command. 
 * 
 */
void Check_salary(char empNumber[MAX_TEXT])
{
    long int result_to_receive = 0;
    int running = 1;
    struct empInfo data;
    struct my_msg_st some_data;
    int msgid;
    //**************************************************************************
    struct resultCol resultPass;
    int client_id;
    client_id = msgget((key_t)2345, 0666 | IPC_CREAT);

    if (client_id == -1)
    {
        fprintf(stderr, "msgget (client_id) failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    //**************************************************************************

    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    if (msgid == -1)
    {
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }
    strcpy(some_data.some_text, empNumber);

    //*********************************************************************************
    int msgid1;
    struct my_cmd comand;
    char bufferCMD[MAX_TEXT] = "CheckSalary";

    msgid1 = msgget((key_t)4231, 0666 | IPC_CREAT);
    if (msgid1 == -1)
    {
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }
    while (running)
    {
        resultPass.reuslt_msg_type = getpid();
        comand.my_sag = 1;
        strcpy(comand.cmd, bufferCMD);
        if (msgsnd(msgid1, (void *)&comand, MAX_TEXT, 0) == -1)
        {
            fprintf(stderr, "msgsnd failed.\n");
            exit(EXIT_FAILURE);
        }

        //********************************************
        some_data.my_msg_type = 1;
        if (msgsnd(msgid, (void *)&some_data, MAX_TEXT, 0) == -1)
        {
            fprintf(stderr, "msgsnd failed\n");
            exit(EXIT_FAILURE);
        }

        int listening = 1;

        while (listening)
        {

            if (msgrcv(client_id, (void *)&resultPass, BUFSIZ, result_to_receive, 0) == -1)
            {
                fprintf(stderr, "msgrcv failed with error: %d\n", errno);
                exit(EXIT_FAILURE);
            }
            printf("\n****************Result************\n\n");
            printf("The Salary of the employee is : %s\n\n", resultPass.result);
            printf("\n**********************************\n");
            listening = 0;
        }

        running = 0;
    }

    if (msgctl(client_id, IPC_RMID, 0) == -1)
    {
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }
}

/**
 *  in response to this command Record-Keeper must return the name of the employee with the
 *  employee number passed as an argument for this command.[Assume that that employee names are distinct: 
 * 
 * 
 */
void Check_employee_number(char empNumber[MAX_TEXT])
{
    long int result_to_receive = 0;
    int running = 1;
    struct empInfo data;
    struct my_msg_st some_data;
    int msgid;
    //**************************************************************************
    struct resultCol resultPass;
    int client_id;
    client_id = msgget((key_t)2345, 0666 | IPC_CREAT);

    if (client_id == -1)
    {
        fprintf(stderr, "msgget (client_id) failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    //**************************************************************************

    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    if (msgid == -1)
    {
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }
    strcpy(some_data.some_text, empNumber);

    //*********************************************************************************
    int msgid1;
    struct my_cmd comand;
    char bufferCMD[MAX_TEXT] = "CheckEmpNum";

    msgid1 = msgget((key_t)4231, 0666 | IPC_CREAT);
    if (msgid1 == -1)
    {
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }
    while (running)
    {
        resultPass.reuslt_msg_type = getpid();
        comand.my_sag = 1;
        strcpy(comand.cmd, bufferCMD);
        if (msgsnd(msgid1, (void *)&comand, MAX_TEXT, 0) == -1)
        {
            fprintf(stderr, "msgsnd failed.\n");
            exit(EXIT_FAILURE);
        }

        //********************************************
        some_data.my_msg_type = 1;
        if (msgsnd(msgid, (void *)&some_data, MAX_TEXT, 0) == -1)
        {
            fprintf(stderr, "msgsnd failed\n");
            exit(EXIT_FAILURE);
        }

        int listening = 1;

        while (listening)
        {

            if (msgrcv(client_id, (void *)&resultPass, BUFSIZ, result_to_receive, 0) == -1)
            {
                fprintf(stderr, "msgrcv failed with error: %d\n", errno);
                exit(EXIT_FAILURE);
            }
            printf("\n****************Result************\n\n");
            printf("Employee Number is : %s\n\n", resultPass.result);
            printf("\n**********************************\n");
            listening = 0;
        }

        running = 0;
    }

    if (msgctl(client_id, IPC_RMID, 0) == -1)
    {
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }
}
/**
 *  In response to this command Record-Keeper must return the employee numbers of all the 
 * employees that work in the department identified by the argumentfor this command. 
 * 
 */
void Check(char empNumber[MAX_TEXT])
{
    long int result_to_receive = 0;
    int running = 1;
    struct empInfo data;
    struct my_msg_st some_data;
    int msgid;
    //**************************************************************************
    struct resultCol resultPass;
    int client_id;
    client_id = msgget((key_t)2345, 0666 | IPC_CREAT);

    if (client_id == -1)
    {
        fprintf(stderr, "msgget (client_id) failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    //**************************************************************************

    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    if (msgid == -1)
    {
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }
    strcpy(some_data.some_text, empNumber);

    //*********************************************************************************
    int msgid1;
    struct my_cmd comand;
    char bufferCMD[MAX_TEXT] = "Check";

    msgid1 = msgget((key_t)4231, 0666 | IPC_CREAT);
    if (msgid1 == -1)
    {
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }
    while (running)
    {
        resultPass.reuslt_msg_type = getpid();
        comand.my_sag = 1;
        strcpy(comand.cmd, bufferCMD);
        if (msgsnd(msgid1, (void *)&comand, MAX_TEXT, 0) == -1)
        {
            fprintf(stderr, "msgsnd failed.\n");
            exit(EXIT_FAILURE);
        }

        //********************************************
        some_data.my_msg_type = 1;
        if (msgsnd(msgid, (void *)&some_data, MAX_TEXT, 0) == -1)
        {
            fprintf(stderr, "msgsnd failed\n");
            exit(EXIT_FAILURE);
        }

        int listening = 1;

        while (listening)
        {

            if (msgrcv(client_id, (void *)&resultPass, BUFSIZ, result_to_receive, 0) == -1)
            {
                fprintf(stderr, "msgrcv failed with error: %d\n", errno);
                exit(EXIT_FAILURE);
            }
            printf("\n****************Result************\n\n");
            printf("Employee's of the Department are : %s\n\n", resultPass.result);
            printf("\n**********************************\n");
            listening = 0;
        }

        running = 0;
    }

    if (msgctl(client_id, IPC_RMID, 0) == -1)
    {
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }
}

/**
 *  in response to this command Record-Keeper must delete the record
*corresponding to the employee number (passed as an argument to the command) from the
*record store. If the operation is successful a 0 is returned; a -1 is returned otherwise. 
 */
void Delete(char empNumber[MAX_TEXT])
{
    long int result_to_receive = 0;
    int running = 1;
    struct empInfo data;
    struct my_msg_st some_data;
    int msgid;
    //**************************************************************************
    struct resultCol resultPass;
    int client_id;
    client_id = msgget((key_t)2345, 0666 | IPC_CREAT);

    if (client_id == -1)
    {
        fprintf(stderr, "msgget (client_id) failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    //**************************************************************************

    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    if (msgid == -1)
    {
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }
    strcpy(some_data.some_text, empNumber);

    //*********************************************************************************
    int msgid1;
    struct my_cmd comand;
    char bufferCMD[MAX_TEXT] = "Delete";

    msgid1 = msgget((key_t)4231, 0666 | IPC_CREAT);
    if (msgid1 == -1)
    {
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }
    while (running)
    {
        resultPass.reuslt_msg_type = getpid();
        comand.my_sag = 1;
        strcpy(comand.cmd, bufferCMD);
        if (msgsnd(msgid1, (void *)&comand, MAX_TEXT, 0) == -1)
        {
            fprintf(stderr, "msgsnd failed.\n");
            exit(EXIT_FAILURE);
        }

        //********************************************
        some_data.my_msg_type = 1;
        if (msgsnd(msgid, (void *)&some_data, MAX_TEXT, 0) == -1)
        {
            fprintf(stderr, "msgsnd failed\n");
            exit(EXIT_FAILURE);
        }

        int listening = 1;

        while (listening)
        {

            if (msgrcv(client_id, (void *)&resultPass, BUFSIZ, result_to_receive, 0) == -1)
            {
                fprintf(stderr, "msgrcv failed with error: %d\n", errno);
                exit(EXIT_FAILURE);
            }
            printf("\n****************Result************\n\n");
            printf("Result : %s\n\n", resultPass.result);
            printf("\n**********************************\n");
            listening = 0;
        }

        running = 0;
    }

    if (msgctl(client_id, IPC_RMID, 0) == -1)
    {
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }
}
int main()
{

    char empName[MAX_TEXT];   // name of the employee
    char empNumber[MAX_TEXT]; // emp
    char empDep[MAX_TEXT];
    char empSalary[MAX_TEXT];
    int cho;
    int numEmp;
    char buffer[BUFSIZ];
    int running = 1;

    printf("For Inserting: [insert] *|* For Checking  Name  : [checkname]   *|* For Checking Depart      : [checkdep] \n");
    printf("For Deleting : [delete] *|* For Checking Salary : [checksalary] *|* For Checking employee No : [ empNum ] \n");
    printf("For Check    : [check]  *|* To End the program  : [end]\n");

    while (running)
    {

        printf("\nPlease: Enter Operation you wanna proform [Operation] :");
        fgets(buffer, BUFSIZ, stdin);

        /**
         * Request the operation 
         * and Requers corrsponding input for each operation 
         */

        if (strncmp(buffer, "insert", 6) == 0)
        {
            printf("\nPlease Enter number of Employees You wanna ADD : ");
            scanf("%d", &numEmp);
            for (int i = 0; i < numEmp; i++)
            {
                printf("\nEnter details of employees %d: \n", i + 1);
                printf("*******************************************************\n");
                printf("Enter Employee's name[Frist_Last]: ");
                scanf("%s", empName);

                printf("Enter employee Number: ");
                scanf("%s", empNumber);

                printf("Enter Departments[use _ as speace] : ");
                scanf("%s", empDep);

                printf("Enter salary: ");
                scanf("%s", empSalary);
                printf("*******************************************************\n");
                Insert(empName, empDep, empNumber, empSalary);
            }
        }
        else if (strncmp(buffer, "checkname", 9) == 0) // condation for checking the name
        {
            printf("Enter employee Number: ");
            scanf("%s", empNumber);
            Check_name(empNumber);
        }
        else if (strncmp(buffer, "checkdep", 8) == 0) // condationg for checking the depatimeent
        {
            printf("Enter employee Number: ");
            scanf("%s", empNumber);
            Check_department(empNumber);
        }
        else if (strncmp(buffer, "checksalary", 11) == 0) // condation for checki
        {
            printf("Check Salary: Enter employee Number: ");
            scanf("%s", empNumber);
            Check_salary(empNumber);
        }
        else if (strncmp(buffer, "check", 5) == 0) //  condationf for checking
        {
            printf("Enter Department Name: ");
            scanf("%s", empNumber);
            Check(empNumber);
        }
        else if (strncmp(buffer, "empNum", 6) == 0)
        {
            printf("Check Employee Number: Enter Employee Name: ");
            scanf("%s", empNumber);
            Check_employee_number(empNumber);
        }
        else if (strncmp(buffer, "delete", 6) == 0)
        {
            printf("Enter Employee Name: ");
            scanf("%s", empNumber);
            Delete(empNumber);
        }
        else if (strncmp(buffer, "end", 3) == 0)
        {
            running = 0;
        }
    }

    exit(EXIT_SUCCESS);
}
