#!/bin/bash
echo "Proceso iniciado, ignorando SIGTERM..."
trap "echo Ignorando SIGTERM" SIGTERM
sleep 20
echo "Proceso terminado normalmente"
