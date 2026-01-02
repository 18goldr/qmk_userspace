# Toggle Gaming Layer
To toggle the gaming layer, I used a similar technique outlined [here](https://docs.qmk.fm/features/layer_lock#combine-layer-lock-with-a-mod-tap).
The difference is that in their example, the tap (quick press) toggles the layer, but I want to do that after holding.

# Changes
- Switch to oneshot shift mod
- Keep shortcuts for control a, s, f, r, v, c, x, z prioritized
  - All other control + character deprioritized
- Slow tap term for alt and gui.
- Add sentence case
- Swap caps lock for caps word on hold
- Adds select region feature like in emacs
- Adds kill line like in emacs

# TODO
- Add a mouse layer for when i want to work one handed
- Set second thumb key to control and first thumb key to meta for emacs.
- Add layer lock for nav
- Disable caps word if use the mouse
- I think the tap term for nav and sym are too short
- Test features on mac
- When I do kill line then hold control and try to do a different combo or layer or something, it doesn't work