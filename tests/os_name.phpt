--TEST--
asyscon.os-name setting
--INI--
asyscon.os-name = OS_NAME
--FILE--
<?php
echo "OS Name: " . OS_NAME . "\n";
?>
--EXPECTREGEX--
OS Name: .+
