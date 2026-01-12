# Actions you can take
Control space (select region start)
Escape (select region end)
Navigation (do nothing)
Type character (insert character and end select region)
Click (select region end)


# Control Flow
## On control space
- keyboard: enable scroll lock
- keyboard: start selection mode
- keyboard: wait 20 ms (or until next cycle of function)
- mouse: detect scroll lock
- mouse: disable scroll lock
- mouse: set select mode flag to true

## On escape
- keyboard: enable scroll lock
- keyboard: disable selection mode
- keyboard: wait 20 ms (or until next cycle of function)
- mouse: detect scroll lock
- mouse: disable scroll lock
- mouse: set select mode flag to false


## On navigation
- keyboard: do navigation

# On type character
- keyboard: type character
- keyboard: enable scroll lock
- keyboard: disable selection mode
- keyboard: wait 20 ms (or until next cycle of function)
- mouse: detect scroll lock
- mouse: disable scroll lock
- mouse: set select mode flag to false


# On click
- mouse: if in selection mode:
  - mouse: click shift
  - mouse: click left button
  - mouse: enable scroll lock
  - mouse: wait 20 ms (or until next cycle of function)
  - keyboard: detect scroll lock
  - keyboard: disable scroll lock
  - keyboard: set select mode flag to false
- mouse: if not in selection mode:
  - mouse: click left button