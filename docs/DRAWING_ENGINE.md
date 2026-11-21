# LIVING CANVAS — Drawing Engine

The C++17 `DrawingEngine`, `Stroke`, and `BrushEngine` preserve every raw input point (`x`, `y`, `pressure`, `timestamp_ms`), color hex, brush type (`Pencil`, `Pen`, `Marker`, `SoftBrush`, `Eraser`), layer assignment, and z-order.

Drawings are never flattened into static bitmaps; strokes remain individually selectable, transformable (`translate`, `scale`, `rotate`), riggable, and editable after saving and reopening from SQLite.

