<?php
$arr = [];//初始化
echo $arr;
$arr[] = 'foo';//无key赋值
echo $arr;
$arr[2] = 'abc';//数字key赋值
echo $arr;
$arr['a'] = 'bar';//字符串key赋值
echo $arr;
$arr[] = 'xyz';//无key赋值
echo $arr;
$arr['a'] = 'foo';//已存在key赋值
echo $arr;
echo $arr['a'];//根据key查找
unset($arr['a']);//删除key
echo $arr;
