#!/usr/bin/env bash

NUM=10
DELAY=0.05
EXTS=("txt" "bin" "dat" "jpg" "log")

rm -rf test_original test_encrypted test_decrypted *.log 2>/dev/null
mkdir -p test_original test_encrypted test_decrypted

./server > server.log 2>&1 &
SERVER_PID=$!
sleep 2

echo "Генерация файлов разных типов и размеров..."
for i in $(seq 1 $NUM); do
    EXT=${EXTS[$((RANDOM % ${#EXTS[@]}))]}
    SIZE=$((RANDOM % 5000 + 1))
    f="test_original/file_$i.$EXT"
    
    dd if=/dev/urandom of="$f" bs=1024 count=$SIZE status=none
done

echo "Шифрование..."
CLIENT_PIDS=""
for f in test_original/*; do
    NAME=$(basename "$f")
    ./client "$f" "test_encrypted/$NAME.enc" "key$NAME" &
    CLIENT_PIDS="$CLIENT_PIDS $!"
    sleep $DELAY
done
wait $CLIENT_PIDS

echo "Дешифрование..."
CLIENT_PIDS=""
for f in test_encrypted/*; do
    NAME=$(basename "$f" .enc)
    ./client "$f" "test_decrypted/$NAME" "key$NAME" &
    CLIENT_PIDS="$CLIENT_PIDS $!"
    sleep $DELAY
done
wait $CLIENT_PIDS

echo -e "\nРезультат проверки:"
success=0
for f in test_original/*; do
    NAME=$(basename "$f")
    if cmp -s "$f" "test_decrypted/$NAME"; then
        echo "[OK] $NAME ($(du -h "$f" | cut -f1))"
        ((success++))
    else
        echo "[FAIL] $NAME"
    fi
done

echo "---"
echo "Успешно: $success / $NUM"

kill $SERVER_PID 2>/dev/null
wait $SERVER_PID 2>/dev/null
echo "Тест завершен."
