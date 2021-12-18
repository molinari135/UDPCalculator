# UDPCalculator
A simple UDP calculator in C (Read [Notes](#notes) for client's <b>input parameters</b>)

## Screenshots
### Client
![image](https://user-images.githubusercontent.com/64893048/146655948-61558b62-294e-4fd8-badb-b82b87fc0bf0.png)

### Server
![image](https://user-images.githubusercontent.com/64893048/146655957-4a7d0dda-bfca-47d9-8ad4-221800f258dd.png)

## Description
UDP client/server application where the server is a remote calculator that answers to client's queries.
1. Client starts asking server's name and port number like this: `srv.di.uniba.it:56700`
2. Client reads the operation, with this format: `+ 23 45`
3. Client sends to server the query
4. When it receives data, server shows a message like this: `Richiesta operazione '+ 23 45' dal client pippo.di.uniba.it, ip 193.204.187.154`
5. Server execute the operation and sends the result like this:
    - `23 + 45 = 68` for addition;
    - `23 / 45 = 0.51` for division;
    - `23 * 45 = 1035` for multiplication;
    - `23 - 45 = -22` for subtraction;
6. Client reads the answer and prints a message (`Ricevuto risultato dal server srv.di.uniba.it, ip 193.204.187.166: 23 + 45 = 68`)
7. Client reads from standard input the next query
8. If it founds `=`, client ends its process; otherwise goes back to step 2
9. Server never ends its process

## Requirements
- Applicative protocol infos are inside header file `.h`
- Math functions: `add()`, `mult()`, `sub()`, `division()`
- Single line input insertion (`+ 23 45[enter]`, not `+[enter]23[enter]45[enter]`)
- Client and server should be able to start also with default values
- Queries are independant (no cumulative results)

## Notes
- <b>Server's name</b>: localhost
- <b>Server's port</b>: 45000
- Client will connect with these default values if wrong input
- Client also accept args values (insert by IDE)
