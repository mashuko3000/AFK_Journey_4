#!/usr/bin/env bash

NUM=100
DELAY=0.0

TIMEOUT=10

rm -rf test_original test_encrypted test_decrypted *.log 2>/dev/null
mkdir -p test_original test_encrypted test_decrypted

./server > server.log 2>&1 &
SERVER_PID=$!

sleep 3

for i in $(seq 1 $NUM); do
    f="test_original/file_$i.txt"
    echo -e "Файл №$i\nДата: $(date)\nКонтент: $RANDOM$RANDOM" > "$f"
done

echo "Шифрование..."
CLIENT_PIDS=""
for i in $(seq 1 $NUM); do
    ./client "test_original/file_$i.txt" "test_encrypted/file_$i.enc" "key$i" &
    CLIENT_PIDS="$CLIENT_PIDS $!"
    sleep $DELAY
done
wait $CLIENT_PIDS

echo "Дешифрование..."
CLIENT_PIDS=""
for i in $(seq 1 $NUM); do
    ./client "test_encrypted/file_$i.enc" "test_decrypted/file_$i.dec" "key$i" &
    CLIENT_PIDS="$CLIENT_PIDS $!"
    sleep $DELAY
done
wait $CLIENT_PIDS

echo -e "\nРезультат проверки:"
success=0
for i in $(seq 1 $NUM); do
    if cmp -s "test_original/file_$i.txt" "test_decrypted/file_$i.dec"; then
        echo "[OK] Файл $i совпадает"
        ((success++))
    else
        echo "[FAIL] Файл $i различается!"
    fi
done

echo "---"
echo "Успешно пройдено: $success / $NUM"

kill $SERVER_PID 2>/dev/null
wait $SERVER_PID 2>/dev/null

echo "Тест завершен."
