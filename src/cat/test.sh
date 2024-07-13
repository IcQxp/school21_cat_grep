COUNTER_SUCCESS=0
COUNTER_FAIL=0
GNU_CHECK=0
DIF_RES=""
TC_NAME=""
FLAGS=""
FILE=""
TEST_CASE=""

function clear_vars() {
  FLAGS=""
  FILE=""
}

function run_utils() {
  $TEST_CASE > orig_$TC_NAME.log
  ./s21_$TEST_CASE > s21_$TC_NAME.log
}

function compare_results() {
  
  DIF_RES="$(diff -s orig_$TC_NAME.log s21_$TC_NAME.log)"
  if [ "$DIF_RES" == "Files orig_$TC_NAME.log and s21_$TC_NAME.log are identical" ]
    then
      echo -e "Test-$TC_NAME: \033[32m  Success\033[0m"
      (( COUNTER_SUCCESS++ ))

      echo "-----------------------------------------------------------------" >> main_cat_report.log
      echo "" >> main_cat_report.log
      echo "Test:$TC_NAME:  Success" >> main_cat_report.log
      echo "orig: $TEST_CASE" >> main_cat_report.log
      if [ $GNU_CHECK == 1 ] ; then
        echo orig_$TC_NAME.log >> main_cat_report.log
      else
        $TEST_CASE >> main_cat_report.log
      fi
      echo "s21_: ./s21_$TEST_CASE" >> main_cat_report.log
      ./s21_$TEST_CASE >> main_cat_report.log
    else
      echo -e "Test:$TC_NAME: \033[31m  Fail\033[0m"
      (( COUNTER_FAIL++ ))

      echo "-----------------------------------------------------------------" >> main_cat_report.log
      echo "" >> main_cat_report.log
      echo "Test:$TC_NAME:   Fail" >> main_cat_report.log
      echo "orig: $TEST_CASE" >> main_cat_report.log
      if [ $GNU_CHECK == 1 ] ; then
        echo orig_$TC_NAME.log >> main_cat_report.log
      else
        $TEST_CASE >> main_cat_report.log
      fi
      echo "s21_: ./s21_$TEST_CASE" >> main_cat_report.log
      ./s21_$TEST_CASE >> main_cat_report.log
      echo "---------------------------[DIFF]---------------------------" >> main_cat_report.log
      echo "diff -s orig_$TC_NAME.log s21_$TC_NAME.log" >> main_cat_report.log
      echo "$DIF_RES" >> main_cat_report.log
  fi
}

function run_test_case() {
  run_utils
  compare_results
}

touch main_cat_report.log
clear

echo "---------------REPORT-OF-FUNCTIONAL-TEST-S21_CAT-------------------" >> main_cat_report.log

echo -e "Test:"

make

#-------------------------------------------------------------------
clear_vars
TC_NAME="CB1"
FLAGS="-b"
FILE="sample.txt"
TEST_CASE="cat $FLAGS $FILE"
run_test_case
#-------------------------------------------------------------------
clear_vars
TC_NAME="E"
FLAGS="-e"
FILE="sample.txt"
TEST_CASE="cat $FLAGS $FILE"
run_test_case
#-------------------------------------------------------------------
clear_vars
TC_NAME="N"
FLAGS="-n"
FILE="sample.txt"
TEST_CASE="cat $FLAGS $FILE"
run_test_case
#-------------------------------------------------------------------
clear_vars
TC_NAME="S"
FLAGS="-s"
FILE="sample.txt"
TEST_CASE="cat $FLAGS $FILE"
run_test_case
#-------------------------------------------------------------------
clear_vars
TC_NAME="T"
FLAGS="-t"
FILE="sample.txt"
TEST_CASE="cat $FLAGS $FILE"
run_test_case
#-------------------------------------------------------------------
clear_vars
TC_NAME="V"
FLAGS="-v"
FILE="sample.txt"
TEST_CASE="cat $FLAGS $FILE"
run_test_case
#-------------------------------------------------------------------
clear_vars
TC_NAME="BENSTV"
FLAGS="-benstv"
FILE="sample.txt"
TEST_CASE="cat $FLAGS $FILE"
run_test_case
#-------------------------------------------------------------------

GNU_CHECK=1

#-------------------------------------------------------------------
clear_vars
TC_NAME="BGNU"
FLAGS="--number-nonblank"
FILE="sample.txt"
TEST_CASE="cat $FLAGS $FILE"

./s21_$TEST_CASE > s21_$TC_NAME.log
cp ../../datasets/gnu_samples/sample$FLAGS.txt orig_$TC_NAME.log

compare_results
#-------------------------------------------------------------------
clear_vars
TC_NAME="EGNU"
FLAGS="-E"
FILE="sample.txt"
TEST_CASE="cat $FLAGS $FILE"

./s21_$TEST_CASE > s21_$TC_NAME.log
cp ../../datasets/gnu_samples/sample$FLAGS.txt orig_$TC_NAME.log

compare_results
#-------------------------------------------------------------------
clear_vars
TC_NAME="NGNU"
FLAGS="--number"
FILE="sample.txt"
TEST_CASE="cat $FLAGS $FILE"

./s21_$TEST_CASE > s21_$TC_NAME.log
cp ../../datasets/gnu_samples/sample$FLAGS.txt orig_$TC_NAME.log

compare_results
#-------------------------------------------------------------------
clear_vars
TC_NAME="SGNU"
FLAGS="--squeeze-blank"
FILE="sample.txt"
TEST_CASE="cat $FLAGS $FILE"

./s21_$TEST_CASE > s21_$TC_NAME.log
cp ../../datasets/gnu_samples/sample$FLAGS.txt orig_$TC_NAME.log

compare_results
#-------------------------------------------------------------------
clear_vars
TC_NAME="TGNU"
FLAGS="-T"
FILE="sample.txt"
TEST_CASE="cat $FLAGS $FILE"

./s21_$TEST_CASE > s21_$TC_NAME.log
cp ../../datasets/gnu_samples/sample$FLAGS.txt orig_$TC_NAME.log

compare_results
#-------------------------------------------------------------------

echo -e "Result:"
echo "SUCCESS: $COUNTER_SUCCESS"
echo "FAIL: $COUNTER_FAIL"

echo "" >> main_cat_report.log
echo "Result:" >> main_cat_report.log
echo "SUCCESS: $COUNTER_SUCCESS" >> main_cat_report.log
echo "FAIL: $COUNTER_FAIL" >> main_cat_report.log

if [ -d logs ]; then
  rm -rf ./logs/
fi

if ! [ -d logs ]; then
  mkdir ./logs/
fi

mv ./*.log ./logs/