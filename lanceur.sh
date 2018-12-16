#!/bin/bash


a="init"
err=0

while [ $a != "vide" ]
do
    if [ $err -eq '1' ]
    then
        echo "ERR: Veuillez saisir une option valide s'il vous plait !"
    fi
    echo "Voulez vous lancer :"
    echo "* 1 le serveur"
    echo "* 2 le client"
    echo "* 3 les deux"
    read a
    mode=$a
    a=$( echo vide$a | tr -d "[1-3]" )
    err=1
done

#on recupere le port

a="init"
err=0

while [ $a != "vide" ]
do
    if [ $err -eq '1' ]
    then
        echo "ERR: Veuillez saisir un chiffre s'il vous plait !"
    fi
    echo "Veuillez saisir le numero de port :"
    read a
    PORT=$a
    a=$( echo vide$a | tr -d "[0-9]" )
    err=1
done







if [ $mode -eq '1' ]
then
    #SERVEUR
    #on execute le serveur
    cd serveur/sources/
    make run PORT=$PORT
else
    if [ $mode -eq '2' ]
    then
        #CLIENT
        a="init"
        err=0

        while [ $a != "~" ]
        do
            if [ $err -eq '1' ]
            then
                echo "ERR: Veuillez saisir un nom d'hote valide s'il vous plait !"
            fi
            echo "Veuillez saisir le nom d'hote :"
            read a
            HOST=$a
            a=$( echo ~$a | tr -d "[a-zA-Z0-9.]" )
            err=1
        done


        #on execute le client
        cd client/sources/
        make run HOST=$HOST PORT=$PORT
    else
        if [ $mode -eq '3' ]
        then
            #LES DEUX
            a="init"
            err=0

            

            while [ $a != "~" ]
            do
                if [ $err -eq '1' ]
                then
                    echo "ERR: Veuillez saisir un nom d'hote valide s'il vous plait !"
                fi
                echo "Veuillez saisir le nom d'hote :"
                read a
                HOST=$a
                a=$( echo ~$a | tr -d "[a-zA-Z0-9.]" )
                err=1
            done
            x-terminal-emulator --command='cd serveur/sources;make run PORT='$PORT''&
            sleep 2
            echo ""
            echo ""
            echo ""
            echo ""
            cd client/sources/
            make run HOST=$HOST PORT=$PORT
        fi
    fi
fi
