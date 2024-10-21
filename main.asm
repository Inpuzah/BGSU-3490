Entry:
      CLI
      LDAA #$10      ;00010000
      STAA $0242

      LDAA #$00
      STAA $0272

      LDAA #$F0       ;11110000   -->  bits 4, 5, 6, & 7 = outputs
      STAA $0270      ;changes output

      LDAA #$00        ; RESET LEDS (0000 0000)
      STAA $0270

      LDAA #$10        ; LED 1 ON (0000 0001)
      STAA $0270       ; (LED 1 does not work on our prototyping board, but in theory it SHOULD be on)

      LDAA #$00         ; RESET
      STAA $0270 

      LDAA #$20          ; LED 2 ON (0000 0010)
      STAA $0270 

      LDAA #$00         ; RESET
      STAA $0270 

      LDAA #$40          ; LED 3 ON (0000 0100)
      STAA $0270 

      LDAA #$00         ; RESET
      STAA $0270

      LDAA #$80          ; LED 4 ON (0000 1000)
      STAA $0270

mainLoop:
      BRA mainLoop
