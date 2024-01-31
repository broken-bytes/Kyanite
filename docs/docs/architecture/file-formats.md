---
sidebar_position: 2
---

# File Formats

Kyanite uses a series of propertiary file formats for asset management and delivery.

## Scene Format
Scenes are realised in YAML, providing easy access even from text-editors if visual editing is not wanted/needed.

Each component that is registered from either engine- or user-code, automatically registers serialization and deserialization funtions behind the scenes. 

These functions are later called dynamically to construct the actual entity hierarchy from the yaml files.