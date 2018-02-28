#ifndef _LIBPAXOS_H_
#define _LIBPAXOS_H_

/******************************************************************************/
/* Acceptors and logs                                                         */
/******************************************************************************/

#define NODE_ID_UNDEF 0
#define LOG_SIZE 1024
#define MAX_RETRIES 4 // for wor_capture, wor_write, wor_read

/******************************************************************************/
/* Network                                                                    */
/******************************************************************************/

#define NET               "127.0.0.1"
//#define NET_ADDR          ((unsigned int) 16777343) // = inet_addr(NET)
#define ACCEPTORS_NET     "127.0.0.1"
#define MULTICAST_NET     "224.0.0.1"
#define ACCEPTORS_PORT    7000
#define CLIENTS_PORT_BASE 8000

#define INET_ADDR_NET ((unsigned int) 16777343) // = inet_addr(NET)

// uint16_t htons(uint16_t hostshort);
//   The htons() function converts the unsigned short integer hostshort from
//   host byte order to network byte order.

// HTONS_ACCEPTORS_PORT(acc_id) = htons(ACCEPTORS_PORT + acc_id)
#define ACCEPTORS_PORT_0_NBO    22555 // = htons(ACCEPTORS_PORT + 0)
#define ACCEPTORS_PORT_1_NBO    22811 // = htons(ACCEPTORS_PORT + 1)
#define ACCEPTORS_PORT_2_NBO    23067 // = htons(ACCEPTORS_PORT + 2)
#define ACCEPTORS_PORT_3_NBO    23323 // = htons(ACCEPTORS_PORT + 3)
#define ACCEPTORS_PORT_4_NBO    23579 // = htons(ACCEPTORS_PORT + 4)
#define ACCEPTORS_PORT_5_NBO    23835 // = htons(ACCEPTORS_PORT + 5)

// TODO: #define htons(INADDR_ANY)
// TODO: #define htons(CLIENTS_PORT_BASE + client_id)

/******************************************************************************/
/* Proposals and commands                                                     */
/******************************************************************************/

#define INT_MAX 2147483647
#define PROP_UNDEF  (0)
#define PROP_ONE    (1 << 16)
#define PROP_MAX    (INT_MAX - PROP_ONE)
#define PROP_NEXT_NUM(n) ((((n) >> 16) + 1) << 16)
#define PROP_NEXT_NID(id) ((id) & 0xffff)
#define PROP_NEXT(n,id) (PROP_NEXT_NUM(n) + PROP_NEXT_NID(id))
// #define PROP_WRITTEN (INT_MAX)
// #define PROP_CAPTURED (INT_MAX - 1)
#define ADDR_UNDEF (0) // address 0 is unused

/******************************************************************************/
/* Operation return codes                                                     */
/******************************************************************************/

#define STATUS_OK 0
#define STATUS_ERROR 10
#define STATUS_ALREADYWRITTEN 1
#define STATUS_TIMEOUT 2
#define STATUS_UNWRITTEN 3
#define STATUS_UNKNOWNERROR 4
#define STATUS_UNDEF 6
#define STATUS_PREEMPTED 7
#define STATUS_EOS_ERROR 8 // end of segment
#define STATUS_NOTOWNER 9

#define FALSE 0
#define TRUE 1

#endif /* _LIBPAXOS_H_ */
