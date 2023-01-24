# Index
Added the following values available for SGO node names.

## xgs_scene_object_class 's value:
- [GiantAnt](#GiantAnt)

### GiantAnt
| sgo node name | value type | function | value description |
|---|---|---|---|
|ant_BulletAlive|int|change acid presence time|bulletType = 0, default 180; = 1, 240|
|ant_BulletColor|ptr|change ant acid color|[color parameters](#GiantAnt-BulletColor), all parameters must be present|
|ant_BulletExSet|ptr|change ant acid detail|[exset parameters](#GiantAnt-BulletExSet), all parameters must be present|
|ant_BulletFireHeight|float|change acid launch height||

#### GiantAnt BulletColor
| node index | value type | value description |
|---|---|---|
|0|float|red channel|
|1|float|green channel|
|2|float|blue channel|
|3|float|alpha channel|

#### GiantAnt BulletExSet
| node index | value type | value description |
|---|---|---|
|0|int|multiply this factor by original shot count, this must > 0|
|1|float|multiply this factor by original acid speed|
|2|float|multiply this factor by original acid damage|
|3|float|change acid explosion range|
|4|int|set acid penetration, only 0 and 1|
|5|float|change acid size|
|6|float|change acid hitbox size|
|7|float|change acid hit impulse, maybe useless|

##### [Return to Index](#index)
