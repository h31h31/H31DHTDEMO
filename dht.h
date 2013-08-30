
#include "typeinfo"
#define _O_RDWR         0x0002  /* open for reading and writing */
#define _O_CREAT        0x0100  /* create and open file */
#define _O_BINARY       0x8000  /* file mode is binary (untranslated) */



typedef void dht_callback(void *closure, int event,unsigned char *info_hash,void *data, size_t data_len);

#define DHT_EVENT_NONE 0
#define DHT_EVENT_VALUES 1
#define DHT_EVENT_VALUES6 2
#define DHT_EVENT_SEARCH_DONE 3
#define DHT_EVENT_SEARCH_DONE6 4
#define DHT_EVENT_PONG_VALUES 5
#define DHT_EVENT_ANNOUNCE_PEER_VALUES 6
#define DHT_EVENT_FINDNODE_VALUES 7
#define DHT_EVENT_GETPEER_VALUES 8


int dht_init(int s, int s6, const unsigned char *id, const unsigned char *v);
int dht_insert_node(const unsigned char *id, struct sockaddr *sa, int salen);
int dht_ping_node(struct sockaddr *sa, int salen);
int dht_periodic(const void *buf, size_t buflen,const struct sockaddr *from, int fromlen,time_t *tosleep, dht_callback *callback, void *closure);
void make_tid(unsigned char *tid_return, const char *prefix, unsigned short seqno);
int send_get_peers(const struct sockaddr *sa, int salen,unsigned char *tid, int tid_len,unsigned char *infohash, int want, int confirm);
int send_find_node(const struct sockaddr *sa, int salen,const unsigned char *tid, int tid_len,const unsigned char *target, int want, int confirm);
int dht_search(const unsigned char *id, int port, int af, dht_callback *callback, void *closure);
int dht_nodes(int af,int *good_return, int *dubious_return, int *cached_return,int *incoming_return);
void dht_dump_tables(FILE *f);
int dht_get_nodes(struct sockaddr_in *sin, int *num,int *numno,struct sockaddr_in6 *sin6, int *num6, int *num6no);
int dht_uninit(void);

int random(void);
/* This must be provided by the user. */
int dht_blacklisted(const struct sockaddr *sa, int salen);
void dht_hash(unsigned char *hash_return, int hash_size,char *v1, int len1,char *v2, int len2,char *v3, int len3);
int dht_random_bytes(void *buf, size_t size);

