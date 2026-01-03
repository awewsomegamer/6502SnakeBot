; -----------------------------------------------------------------------------
; 6502SnakeBot - 6502 snake robot competition
; File: main.asm
;
; Part of the 6502SnakeBot project (https://github.com/rainbain/6502SnakeBot)
;
; Copyright (c) 2026 Samuel Fitzsimons
;
; This file is licensed under the MIT License.
; See the LICENSE file in the project root for full license terms.
;
; Golden implementation example.
; -----------------------------------------------------------------------------

STATE_CONTROL         = $5000
STATE_SNAKE_DIRECTION = $5001

ISR_IRQ_L = $FFFE
ISR_IRQ_H = $FFFF

entry_point:
    ; Load IRQ
    LDA #>IRQ
    STA ISR_IRQ_H
    LDA #<IRQ
    STA ISR_IRQ_L

    ; Enable interrupts
    CLI
LOOP:
    JMP LOOP

IRQ:
    ; Snake Up
    LDA #$04
    STA STATE_SNAKE_DIRECTION

    ; Acknowledge interrupts.
    LDA #$02
    STA STATE_CONTROL
    RTI