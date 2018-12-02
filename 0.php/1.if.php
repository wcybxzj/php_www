<?php

function test1($a,$b)
{
	echo "test1:";
	echo "a:".$a.",b:".$b;
	if (!($a&&$b)) {
		echo "输出".PHP_EOL;
	}else{
		echo "空".PHP_EOL;
	}
}

function test2($a,$b)
{
	echo "test2:";
	echo "a:".$a.",b:".$b;
	if (!$a&&!$b) {
		echo "输出".PHP_EOL;
	}else{
		echo "空".PHP_EOL;
	}
}
//test1:a:1,b:1空
//test2:a:1,b:1空
//test1:a:1,b:0输出
//test2:a:1,b:0空
//test1:a:0,b:1输出
//test2:a:0,b:1空
//test1:a:0,b:0输出
//test2:a:0,b:0输出


test1($a=1,$b=1);
test2($a=1,$b=1);
test1($a=1,$b=0);
test2($a=1,$b=0);
test1($a=0,$b=1);
test2($a=0,$b=1);
test1($a=0,$b=0);
test2($a=0,$b=0);

echo "---------------------".PHP_EOL;
function test3($a,$b)
{
	echo "test3:";
	echo "a:".$a.",b:".$b;
	if (!($a||$b)) {
		echo "输出".PHP_EOL;
	}else{
		echo "空".PHP_EOL;
	}
}

function test4($a,$b)
{
	echo "test4:";
	echo "a:".$a.",b:".$b;
	if (!$a||!$b) {
		echo "输出".PHP_EOL;
	}else{
		echo "空".PHP_EOL;
	}
}

//test3:a:1,b:1空
//test4:a:1,b:1空
//test3:a:1,b:0空
//test4:a:1,b:0输出
//test3:a:0,b:1空
//test4:a:0,b:1输出
//test3:a:0,b:0输出
//test4:a:0,b:0输出
test3($a=1,$b=1);
test4($a=1,$b=1);
test3($a=1,$b=0);
test4($a=1,$b=0);
test3($a=0,$b=1);
test4($a=0,$b=1);
test3($a=0,$b=0);
test4($a=0,$b=0);
