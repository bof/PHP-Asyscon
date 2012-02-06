--TEST--
asyscon.os-release setting
--INI--
asyscon.os-release = OS_RELEASE
--FILE--
<?php
echo "OS Release: " . OS_RELEASE . "\n";
?>
--EXPECTREGEX--
OS Release: .*
