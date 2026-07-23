import sys
from pathlib import Path

base = Path(__file__).resolve().parent
core_folder = base / "build" / "Release"

sys.path.append(str(core_folder))

import bereshitCore
from bereshitCore import GameObject
from bereshitCore import Component
from bereshitCore import Vector3
from bereshitCore import World
from bereshitCore import Rigidbody
from bereshitCore import Collider
from bereshitCore import BoxCollider
from bereshitCore import Quaternion
from bereshitCore import Joint
from bereshitCore import FixedJoint

