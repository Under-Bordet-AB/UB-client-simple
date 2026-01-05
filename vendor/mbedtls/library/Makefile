# Simple Makefile for mbedTLS
# Builds libmbedcrypto.a, libmbedx509.a, libmbedtls.a

CC ?= gcc
CFLAGS ?= -O2 -Wall -I../include -I. -D_FILE_OFFSET_BITS=64
AR ?= ar
ARFLAGS = rcs

# Source definitions
CRYPTO_SRC = \
    aes.c aesni.c aesce.c aria.c asn1parse.c asn1write.c base64.c \
    bignum.c bignum_core.c bignum_mod.c bignum_mod_raw.c block_cipher.c \
    camellia.c ccm.c chacha20.c chachapoly.c cipher.c cipher_wrap.c \
    cmac.c constant_time.c ctr_drbg.c des.c dhm.c ecdh.c ecdsa.c \
    ecjpake.c ecp.c ecp_curves.c ecp_curves_new.c entropy.c entropy_poll.c \
    error.c gcm.c hkdf.c hmac_drbg.c lmots.c lms.c md.c md5.c \
    memory_buffer_alloc.c nist_kw.c oid.c padlock.c pem.c pk.c pk_ecc.c \
    pk_wrap.c pkcs12.c pkcs5.c pkparse.c pkwrite.c platform.c platform_util.c \
    poly1305.c psa_crypto.c psa_crypto_aead.c psa_crypto_cipher.c \
    psa_crypto_client.c psa_crypto_driver_wrappers_no_static.c \
    psa_crypto_ecp.c psa_crypto_ffdh.c psa_crypto_hash.c psa_crypto_mac.c \
    psa_crypto_pake.c psa_crypto_rsa.c psa_crypto_se.c \
    psa_crypto_slot_management.c psa_crypto_storage.c psa_its_file.c \
    psa_util.c ripemd160.c rsa.c rsa_alt_helpers.c sha1.c sha256.c \
    sha512.c sha3.c threading.c timing.c version.c version_features.c

# 3rd Party Crypto (removed - not needed for minimal TLS client)

X509_SRC = \
    x509.c x509_create.c x509_crl.c x509_crt.c x509_csr.c \
    x509write.c x509write_crt.c x509write_csr.c pkcs7.c

TLS_SRC = \
    debug.c mps_reader.c mps_trace.c net_sockets.c ssl_cache.c \
    ssl_ciphersuites.c ssl_client.c ssl_cookie.c ssl_debug_helpers_generated.c \
    ssl_msg.c ssl_ticket.c ssl_tls.c ssl_tls12_client.c ssl_tls12_server.c \
    ssl_tls13_keys.c ssl_tls13_client.c ssl_tls13_server.c ssl_tls13_generic.c

# Object conversions
CRYPTO_OBJS = $(CRYPTO_SRC:.c=.o)
X509_OBJS = $(X509_SRC:.c=.o)
TLS_OBJS = $(TLS_SRC:.c=.o)

.PHONY: all clean

all: libmbedcrypto.a libmbedx509.a libmbedtls.a

libmbedcrypto.a: $(CRYPTO_OBJS)
	$(AR) $(ARFLAGS) $@ $^

libmbedx509.a: $(X509_OBJS)
	$(AR) $(ARFLAGS) $@ $^

libmbedtls.a: $(TLS_OBJS)
	$(AR) $(ARFLAGS) $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.a *.o
