all: asn1 client

asn1:
	rm -rf asn1_source
	mkdir asn1_source
	asn1c rrc.asn1 -D asn1_source -pdu=RRCConnectionSetup -no-gen-OER -no-gen-example -fcompound-names -fno-include-deps

client:
	rm -rf build
	mkdir build
	gcc -Iasn1_source main.c client.c asn1_source/*.c -o build/client -lsctp -DPDU=RRCConnectionSetup -DASN_DISABLE_OER_SUPPORT

start:
	./build/client

clean:
	rm -rf build
	rm -rf asn1_source