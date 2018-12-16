SERVEUR=../bin/serveur 
CLIENT=../bin/client 
CHEMIN_CLIENT=client/sources/
CHEMIN_SERVEUR=serveur/sources/

all: $(CLIENT) $(SERVEUR) 

$(CLIENT):
	@(cd $(CHEMIN_CLIENT) && $(MAKE))

$(SERVEUR):
	@(cd $(CHEMIN_SERVEUR) && $(MAKE))

.PHONY: run clean

run:
	./lanceur.sh

clean:
	cd $(CHEMIN_CLIENT) && $(MAKE) $@
	cd $(CHEMIN_SERVEUR) && $(MAKE) $@
