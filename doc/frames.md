# Frames

## Phonemes

## Codes

| Full name (<= 11) | ID          | Short code (<= 3) | Description                                                  |
| ----------------- | ----------- | ----------------- | ------------------------------------------------------------ |
| Pause 0ms         | 0           | P0                | A long pause.                                                |
| Pause 100ms       | 1           | P10               | A long pause.                                                |
| Pause 200ms       | 2           | P20               | A long pause.                                                |
| Pause 700ms       | 3           | P70               | A long pause.                                                |
| Wait 30ms         | 4           | W3                | A short pause.                                               |
| Wait 60ms         | 5           | W6                | A short pause.                                               |
| Wait 90ms         | 6           | W9                | A short pause.                                               |
| Fast              | 7           | FA                | Play next phoneme or group at twice the speed.               |
| Slow              | 8           | SL                | Play next phoneme or group at two thirds the speed.          |
| Stress            | 14          | STS               | Adds stress to the next phoneme or group.                    |
| Relax             | 15          | RLX               | Adds relaxation to the next phoneme or group.                |
| Repeat X          | 26          | 00-FF×            | Repeats x times the next phoneme.                            |
| Delay X           | 30          | D00-FF            | Waits some time, by 10 ms intervals.                         |
| Pitch X           | 32 (custom) | ΔP                | Adjust the pitch relative to the base pitch to the next phoneme or group. |
| Group open        | 33 (custom) | <                 | Starts a group. All preceding modifiers will be applied to every phoneme in the group. |
| Group end         | 34 (custom) | >                 | Ends a group. All preceding modifiers will be applied to every phoneme in the group. |
| End               | 0xFF        | -                 | End of the word.                                             |
|                   |             |                   |                                                              |
|                   |             |                   |                                                              |
|                   |             |                   |                                                              |

