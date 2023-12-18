# Capture the current system time
Get-Date -Format "yyyy-MM-dd HH:mm:ss" > log.txt

pio device monitor | Tee-Object -File "log.txt" -Append