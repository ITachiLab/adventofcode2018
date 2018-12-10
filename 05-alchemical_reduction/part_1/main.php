<?php

function invert($str) {
    $unit = ord($str);

    if ($unit >= 65 && $unit <= 90) {
        return strtolower(chr($unit));
    } else if ($unit >= 97 && $unit <= 122) {
        return strtoupper(chr($unit));
    }
}

$input = fopen('input.txt', 'r');
$reductionResult = array();

while (!feof($input)) {
    $unit = fread($input, 1);

    if (invert(end($reductionResult)) == $unit) {
        array_pop($reductionResult);
    } else {
        $reductionResult[] = $unit;
    }
}

echo count($reductionResult) . "\n";

?>
