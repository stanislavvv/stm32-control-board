# Some plans

## Testing

  * test shell processing for overflow by very long command (several times more, than input buffer)

## Flow

  * CI/CD in private git (run `make test`, `make docs`, `make` and `make bin && make hex` in sequence)
  * CI/CD in github (very low priority)

## Programming

  * Refactor all "@todo marks"
  * Refactor/rewrite SPI and graphic LCD code - does not work now
  * Move uart communication from shell to separate task(s?)

