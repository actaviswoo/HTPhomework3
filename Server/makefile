all: asn1 server

asn1:
	rm -rf asn1_source
	mkdir asn1_source
	asn1c rrc.asn1 -D asn1_source -pdu=RRCConnectionSetup -no-gen-OER -no-gen-example -fcompound-names -fno-include-deps

server:
	rm -rf build
	mkdir build
	gcc -Iasn1_source main.c server.c asn1_source/*.c -o build/server -lsctp -DPDU=RRCConnectionSetup -DASN_DISABLE_OER_SUPPORT

start:
	sh port_clear.sh
	./build/server

clean:
	sh port_clear.sh
	rm -rf build
	rm -rf asn1_source