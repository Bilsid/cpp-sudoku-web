<?php
// Get the level (1-4) OR the custom number (e.g., 45)
$level_input = isset($_GET['level']) ? $_GET['level'] : 3;

// If they passed "custom", check for the 'holes' parameter
if ($level_input == 'custom') {
    $holes = isset($_GET['holes']) ? intval($_GET['holes']) : 50;
    // Pass the raw number (e.g., 55) directly to C++
    $arg = $holes;
} else {
    // Normal 1-4 levels
    $arg = intval($level_input);
}

// Security Check: Ensure input is just a number
if (!is_numeric($arg)) { $arg = 3; }

// Execute C++
$command = "./sudoku_gen " . $arg;
$output = shell_exec($command);
echo $output;
?>
