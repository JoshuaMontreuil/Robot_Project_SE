#
# Robot v1 C - Makefile principal.
# 
# @author Matthias Brun, adapté par Jérôme Delatour
#

# Répertoire d'installation de la librairie Infox
# pour la compilation avec le simulateur Intox 
# TODO (à adapter) :
# INTOXDIR = $(realpath /<path>/<to>/infox_prose-<arch>-v0.3
# Exemple :
# Attention : pas d'espace ou d'accent dans votre chemin
# sinon passer par realpath ne fonctionne pas
INTOXDIR = $(realpath /home/joshua/Documents/GitHub/Robot_Project_SE/infox_prose-x86_64-v0.3/)


# Organisation des sources.
#

export SRCDIR = src
export BINDIR = bin

SUBDIRS = $(SRCDIR)

#
# Définitions des outils.
#

# Compilateur.

export CC = gcc

# options de compilation
export CCFLAGS += -O0 
# avec debuggage : -g -DDEBUG
# sans debuggage : -DNDEBUG
export CCFLAGS += -DNDEBUG
 # gestion automatique des dépendances
export CCFLAGS += -MMD -MP
export CCFLAGS += -D_BSD_SOURCE -D_XOPEN_SOURCE_EXTENDED -D_XOPEN_SOURCE -D_DEFAULT_SOURCE -D_GNU_SOURCE
#On compile en utilisant les options de gcc
export CCFLAGS += -std=c99 -Wall
# -pedantic retiré car génère des warnings pour mes TRACE
export LDFLAGS += -lrt -pthread

# options de compilation pour l'utilisation de Intox/Infox
export CCFLAGS += -DINTOX
export CCFLAGS += -I$(INTOXDIR)/include/infox/prose/
export LDFLAGS += -L$(INTOXDIR)/lib/ -linfox

#
# Définitions du binaire à générer.
#
export PROG = $(BINDIR)/robot_pc

#
# Règles du Makefile.
#

# Compilation récursive.
all: 
	@for i in $(SUBDIRS); do (cd $$i; make $@); done

# Nettoyage.
.PHONY: clean

clean:
	@for i in $(SUBDIRS); do (cd $$i; make $@); done
	@rm -f $(PROG) core* $(BINDIR)/core*

