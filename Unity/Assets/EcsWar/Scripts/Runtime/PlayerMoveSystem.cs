using Unity.Entities;
using Unity.Mathematics;
using Unity.Transforms;

namespace EcsWar {

    public partial class PlayerMoveSystem : SystemBase {

        protected override void OnUpdate() {
            Entities
                .ForEach((ref MoveData pv, in PlayerInfo player, in LocalTransform lts, in ActiveInput activeInput) => {
                    // move player
                    if (activeInput.Left) {
                        pv.Angular = new float3(0, -player.RotationSpeed, 0);
                    }
                    else if (activeInput.Right) {
                        pv.Angular = new float3(0, player.RotationSpeed, 0);
                    }
                    else {
                        pv.Angular = new float3(0, 0, 0);
                    }

                    var pos = float3.zero;

                    if (activeInput.Accelerate) {
                        pos.z = player.MoveSpeed;
                    }
                    else if (activeInput.Reverse) {
                        pos.z = -player.MoveSpeed;
                    }

                    pv.Linear = math.mul(lts.Rotation, pos);
                }).WithoutBurst().Run();
        }
    }
}