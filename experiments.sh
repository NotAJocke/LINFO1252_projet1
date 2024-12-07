#!/bin/bash

bash src/performance/performance.sh
# bash src/performance/performance_mutex.sh
# bash src/performance/performance_custom.sh

#!/bin/bash

# Run the performance script
# bash src/performance/performance.sh
# bash src/performance/performance_mutex.sh
bash src/performance/performance_custom.sh

# Print the content of each CSV file
echo "Content of performance_tts_custom.csv:"
cat src/performance/performance_tts_custom.csv
echo

echo "Content of performance_ts_custom.csv:"
cat src/performance/performance_ts_custom.csv
echo

echo "Content of performance_p1_custom.csv:"
cat src/performance/performance_p1_custom.csv
echo
