/* Here's the receiver program. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_TEXT 512
struct my_msg_st
{
    long int my_msg_type;
    char some_text[BUFSIZ];
};

struct my_cmd
{
    long int my_sag;
    char cmd[BUFSIZ];
};

struct resultCol
{
    long int reuslt_msg_type;
    char result[BUFSIZ];
};

struct empInfo
{
    char empName[BUFSIZ];
    char empDep[BUFSIZ];
    char employeeNumber[BUFSIZ];
    char salary[BUFSIZ];
};

int main()
{
    int running = 1;
    int msgid;
    int msgid1;
    struct empInfo indvInf[100];
    struct my_msg_st some_data;
    long int msg_to_receive = 0;
    long int cmd_to_receive = 0;
    char cmd2[BUFSIZ] = "";
    struct my_cmd comand;
    //****************************************************

    /**-------------------------------------------------*/
    msgid1 = msgget((key_t)4231, 0666 | IPC_CREAT);
    if (msgid1 == -1)
    {
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }
    /* First, we set up the message queue. */
    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);

    if (msgid == -1)
    {
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    /* Then the messages are retrieved from the queue, until an end message is encountered.
     Lastly, the message queue is deleted. */
    int i = 0;
    int j = 0;

    while (running)
    {

        if (msgrcv(msgid1, (void *)&comand, BUFSIZ,
                   cmd_to_receive, 0) == -1)
        {
            fprintf(stderr, "msgrcv failed with error 1 : %d\n", errno);
            exit(EXIT_FAILURE);
        }

        if (strncmp(comand.cmd, "Insert", 6) == 0)
        {
            switch (i)
            {
            case 0:
                if (msgrcv(msgid, (void *)&some_data, BUFSIZ,
                           msg_to_receive, 0) == -1)
                {
                    fprintf(stderr, "msgrcv failed with error 2: %d\n", errno);
                    exit(EXIT_FAILURE);
                }

                strcpy(indvInf[j].empName, some_data.some_text);
                printf("Employee Name %d %d: %s \n", i, j, indvInf[j].empName);
                break;
            case 1:
                if (msgrcv(msgid, (void *)&some_data, BUFSIZ,
                           msg_to_receive, 0) == -1)
                {
                    fprintf(stderr, "msgrcv failed with error 3: %d\n", errno);
                    exit(EXIT_FAILURE);
                }

                strcpy(indvInf[j].empDep, some_data.some_text);
                printf("Employee Department %d %d: %s \n", i, j, indvInf[j].empDep);
                break;
            case 2:
                if (msgrcv(msgid, (void *)&some_data, BUFSIZ,
                           msg_to_receive, 0) == -1)
                {
                    fprintf(stderr, "msgrcv failed with error 4: %d\n", errno);
                    exit(EXIT_FAILURE);
                }

                strcpy(indvInf[j].employeeNumber, some_data.some_text);
                printf("Employee Number %d %d: %s \n", i, j, indvInf[j].employeeNumber);
                break;
            case 3:
                if (msgrcv(msgid, (void *)&some_data, BUFSIZ,
                           msg_to_receive, 0) == -1)
                {
                    fprintf(stderr, "msgrcv failed with error 5: %d\n", errno);
                    exit(EXIT_FAILURE);
                }

                strcpy(indvInf[j].salary, some_data.some_text);
                printf("Employee Salary %d %d: %s \n", i, j, indvInf[j].salary);
                printf("*******************************************************\n");
                j++;
                i = -1;
                break;
            }

            i++;
            if (strncmp(some_data.some_text, "end", 3) == 0)
            {
                running = 0;
            }
        }
        else if (strncmp(comand.cmd, "CheckName", 9) == 0)
        {

            strcpy(comand.cmd, cmd2);
            if (msgrcv(msgid, (void *)&some_data, BUFSIZ,
                       msg_to_receive, 0) == -1)
            {
                fprintf(stderr, "msgrcv failed with error 5: %d\n", errno);
                exit(EXIT_FAILURE);
            }
            printf("Recieved Employee's Number @ Check Name %s\n", some_data.some_text);
            for (int i = 0; i < 100; i++)
            {
                if (strncmp(indvInf[i].employeeNumber, some_data.some_text, sizeof(some_data.some_text)) == 0)
                {
                    printf("Employee's Name : %s\n", indvInf[i].empName);

                    strcpy(some_data.some_text, indvInf[i].empName);
                    int client_id;
                    struct resultCol resultPass;
                    client_id = msgget((key_t)2345, 0666 | IPC_CREAT);

                    if (client_id == -1)
                    {
                        fprintf(stderr, "msgget (client_id) failed with error: %d\n", errno);
                        exit(EXIT_FAILURE);
                    }

                    //Return data to client
                    if (msgsnd(client_id, (void *)&some_data, MAX_TEXT, 0) == -1)
                    {
                        fprintf(stderr, "msgsnd CheckName failed\n");
                        exit(EXIT_FAILURE);
                    }
                    printf("Result Sent\n");
                }
            }
        }
        else if (strncmp(comand.cmd, "CheckDep", 8) == 0)
        {
            //Check_department
            printf("Executing command - Check_department\n");
             strcpy(comand.cmd, cmd2);
            if (msgrcv(msgid, (void *)&some_data, BUFSIZ,
                       msg_to_receive, 0) == -1)
            {
                fprintf(stderr, "msgrcv failed with error 5: %d\n", errno);
                exit(EXIT_FAILURE);
            }
            printf("Recieved Employee's Number @ Check Name %s\n", some_data.some_text);
            for (int i = 0; i < 100; i++)
            {
                if (strncmp(indvInf[i].employeeNumber, some_data.some_text, sizeof(some_data.some_text)) == 0)
                {
                    printf("Employee's Name : %s\n", indvInf[i].empDep);

                    strcpy(some_data.some_text, indvInf[i].empDep);
                    int client_id;
                    struct resultCol resultPass;
                    client_id = msgget((key_t)2345, 0666 | IPC_CREAT);

                    if (client_id == -1)
                    {
                        fprintf(stderr, "msgget (client_id) failed with error: %d\n", errno);
                        exit(EXIT_FAILURE);
                    }

                    //Return data to client
                    if (msgsnd(client_id, (void *)&some_data, MAX_TEXT, 0) == -1)
                    {
                        fprintf(stderr, "msgsnd CheckName failed\n");
                        exit(EXIT_FAILURE);
                    }
                    printf("Result Sent\n");
                }
            }
        }
        else if (strncmp(comand.cmd, "CheckSalary", 11) == 0)
        {
            //Check_salary
            printf("Executing command - Check_salary\n");
             strcpy(comand.cmd, cmd2);
            if (msgrcv(msgid, (void *)&some_data, BUFSIZ,
                       msg_to_receive, 0) == -1)
            {
                fprintf(stderr, "msgrcv failed with error 5: %d\n", errno);
                exit(EXIT_FAILURE);
            }
            printf("Recieved Employee's Number @ Check Name %s\n", some_data.some_text);
            for (int i = 0; i < 100; i++)
            {
                if (strncmp(indvInf[i].employeeNumber, some_data.some_text, sizeof(some_data.some_text)) == 0)
                {
                    printf("Employee's Salary: %s\n", indvInf[i].salary);

                    strcpy(some_data.some_text, indvInf[i].salary);
                    int client_id;
                    struct resultCol resultPass;
                    client_id = msgget((key_t)2345, 0666 | IPC_CREAT);

                    if (client_id == -1)
                    {
                        fprintf(stderr, "msgget (client_id) failed with error: %d\n", errno);
                        exit(EXIT_FAILURE);
                    }

                    //Return data to client
                    if (msgsnd(client_id, (void *)&some_data, MAX_TEXT, 0) == -1)
                    {
                        fprintf(stderr, "msgsnd CheckName failed\n");
                        exit(EXIT_FAILURE);
                    }
                    printf("Result Sent\n");
                }
            }
        }
        else if (strncmp(comand.cmd, "CheckEmpNum", 11) == 0)
        {
            //Check_employee_number
            printf("Executing command - Check_employee_number\n");
             strcpy(comand.cmd, cmd2);
            if (msgrcv(msgid, (void *)&some_data, BUFSIZ,
                       msg_to_receive, 0) == -1)
            {
                fprintf(stderr, "msgrcv failed with error 5: %d\n", errno);
                exit(EXIT_FAILURE);
            }
            for (int i = 0; i < 100; i++)
            {
                if (strncmp(indvInf[i].empName, some_data.some_text, sizeof(some_data.some_text)) == 0)
                {
                    printf("Employee's Number : %s\n", indvInf[i].employeeNumber);

                    strcpy(some_data.some_text, indvInf[i].employeeNumber);
                    int client_id;
                    struct resultCol resultPass;
                    client_id = msgget((key_t)2345, 0666 | IPC_CREAT);

                    if (client_id == -1)
                    {
                        fprintf(stderr, "msgget (client_id) failed with error: %d\n", errno);
                        exit(EXIT_FAILURE);
                    }

                    //Return data to client
                    if (msgsnd(client_id, (void *)&some_data, MAX_TEXT, 0) == -1)
                    {
                        fprintf(stderr, "msgsnd CheckName failed\n");
                        exit(EXIT_FAILURE);
                    }
                    printf("Result Sent\n");
                }
            }
        }
        else if (strncmp(comand.cmd, "Check", 5) == 0)
        {
             //Check_employee_number
            int cnt;
            printf("Executing command - Check\n");
             strcpy(comand.cmd, cmd2);
            if (msgrcv(msgid, (void *)&some_data, BUFSIZ,
                       msg_to_receive, 0) == -1)
            {
                fprintf(stderr, "msgrcv failed with error 5: %d\n", errno);
                exit(EXIT_FAILURE);
            }
            for (int i = 0; i < 100; i++)
            {
                if (strncmp(indvInf[i].empDep, some_data.some_text, sizeof(some_data.some_text)) == 0)
                {
                   

                    strcpy(some_data.some_text, indvInf[i].employeeNumber);
                    int client_id;
                    struct resultCol resultPass;
                    client_id = msgget((key_t)2345, 0666 | IPC_CREAT);

                    if (client_id == -1)
                    {
                        fprintf(stderr, "msgget (client_id) failed with error: %d\n", errno);
                        exit(EXIT_FAILURE);
                    }

                    //Return data to client
                    if (msgsnd(client_id, (void *)&some_data, MAX_TEXT, 0) == -1)
                    {
                        fprintf(stderr, "msgsnd CheckName failed\n");
                        exit(EXIT_FAILURE);
                    }
                    printf("Result Sent\n");
                }
            }
        }
        else if (strncmp(comand.cmd, "Delete", 6) == 0)                                               
        {  
            /***
             * This sectiong is used for the operation of delete
             * it deletes the selected array element and pass 0 if it is succeessed
             * 
             */                 

            printf("Executing command - Delete\n");
             strcpy(comand.cmd, cmd2);
            if (msgrcv(msgid, (void *)&some_data, BUFSIZ,
                       msg_to_receive, 0) == -1)
            {
                fprintf(stderr, "msgrcv failed with error 5: %d\n", errno);
                exit(EXIT_FAILURE);
            }
            printf("Recieved Employee's Name @ Delete %s\n", some_data.some_text);
            for (int i = 0; i < 100; i++)
            {
                if (strncmp(indvInf[i].empName, some_data.some_text, sizeof(some_data.some_text)) == 0)
                {
                    printf("Employee's Name: %s\n", indvInf[i].empName);
                        for(int j = i - 1; j < 100 - 1;j++){
                            strcpy(indvInf[j].empDep,indvInf[j+1].empDep);                          //deletes each invdInf[i] and replace it by indvInf
                            strcpy(indvInf[j].employeeNumber,indvInf[j+1].employeeNumber);
                            strcpy(indvInf[j].empName,indvInf[j+1].empName);
                            strcpy(indvInf[j].salary,indvInf[j+1].salary);
                        }
                     

                    strcpy(some_data.some_text, "0");
                    int client_id;
                    struct resultCol resultPass;
                    client_id = msgget((key_t)2345, 0666 | IPC_CREAT);                               // message queues for 

                    if (client_id == -1)                            
                    {
                        fprintf(stderr, "msgget (client_id) failed with error: %d\n", errno);
                        exit(EXIT_FAILURE);
                    }

                    //Return data to client
                    if (msgsnd(client_id, (void *)&some_data, MAX_TEXT, 0) == -1)
                    {
                        fprintf(stderr, "msgsnd CheckName failed\n");
                        exit(EXIT_FAILURE);
                    }
                    printf("Result Sent\n");
                }
            }
        }
        else
        {
            strcpy(comand.cmd, cmd2);
        }
    }

    if (msgctl(msgid, IPC_RMID, 0) == -1)
    {
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }

    if (msgctl(msgid1, IPC_RMID, 0) == -1)
    {
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }
    printf(" Command Recived : %s\n", comand.cmd);

    exit(EXIT_SUCCESS);
}