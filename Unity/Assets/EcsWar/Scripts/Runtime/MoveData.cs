﻿using System;
using Unity.Entities;
using Unity.Mathematics;

namespace EcsWar {

    [Serializable]
    public struct MoveData : IComponentData {
        public float3 Angular;
        public float3 Linear;
    }
}