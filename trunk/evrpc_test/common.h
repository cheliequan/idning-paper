//#define EV_GET(msg, member) \
        //(msg->member##_data)

#define EV_GET(msg, member) \
        (msg->member)
