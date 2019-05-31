<?php
//tail -f /tmp/test_log.log
for ($i = 0; $i < 20; $i++) {
	$str = date("Y-m-d H:i:s")."this is Imooclog:".rand(1,10000);
	write($str);
	sleep(1);
}
