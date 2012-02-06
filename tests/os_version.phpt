--TEST--
asyscon.os-version setting
--INI--
asyscon.os-version = OS_VERSION
--FILE--
<?php
echo "OS Version: " . OS_VERSION . "\n";
?>
--EXPECTREGEX--
OS Version: .*
