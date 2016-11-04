VPATH 	= src inc lib
CPPFLAGS = -I include
new: clean all

clean: 
	rm -rf *.o */*.o *_app *.a
all: examples

examples: client_encrypt_app client_decrypt_app server_encrypt_app server_decrypt_app

client_encrypt_app: libclient.a
client_decrypt_app: libclient.a

server_encrypt_app: libserver.a
server_decrypt_app: libserver.a

#############################
# Create Client Library
#############################
# libclient.a: libclient.a(client_side.o) libhelpers.a
# libclient.a: libclient.a(client_side.o) libhelpers.a
# libclient.a(client_side.o): client_side.o
# 	$(AR) $(ARFLAGS) $@ $?

libclient.a: libclient.a(client_side.o) libclient.a(helpers.o) libclient.a(encryption_decryption.o)

libclient.a(client_side.o): client_side.o
	$(AR) $(ARFLAGS) $@ $?

libclient.a(helpers.o): helpers.o
	$(AR) $(ARFLAGS) $@ $?

libclient.a(encryption_decryption.o): encryption_decryption.o
	$(AR) $(ARFLAGS) $@ $?

###########################
# Create Server Library
###########################

# libserver.a: libserver.a(server_side.o) libhelpers.a


libserver.a: libserver.a(server_side.o) libserver.a(helpers.o) libserver.a(encryption_decryption.o)

libserver.a(server_side.o): server_side.o
	$(AR) $(ARFLAGS) $@ $?

libserver.a(helpers.o): helpers.o
	$(AR) $(ARFLAGS) $@ $?

libserver.a(encryption_decryption.o): encryption_decryption.o
	$(AR) $(ARFLAGS) $@ $?



client_encrypt_app.o: client_side.h helpers.h
client_decrypt_app.o: client_side.h helpers.h

server_encrypt_app.o: server_side.h helpers.h
server_decrypt_app.o: server_side.h helpers.h 


client_side.o: client_side.h helpers.h encryption_decryption.h
server_side.o: server_side.h helpers.h encryption_decryption.h
helpers.o: helpers.h
encryption_decryption.o: encryption_decryption.h

