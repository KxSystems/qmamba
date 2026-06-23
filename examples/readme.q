qmamba:use `kx.qmamba
// Restart
qmamba.env.list[]
qmamba.create "test"
qmamba.env.list[]
qmamba.activate "test"
qmamba.env.list[]
show .Q.m.SP;
qmamba.auth.login (`HOST`BEARER)!("kxdev.gitlab.io";getenv `GITLAB_TOKEN)
qmamba.search (`SPECS`CHANNEL)!(enlist "*protobuf*";enlist "https://kxdev.gitlab.io/kdbx/conda-channel/channel")
qmamba.install (`SPECS`CHANNEL)!(enlist "q-kx-protobuf";("https://kxdev.gitlab.io/kdbx/conda-channel/channel";"conda-forge"))
qmamba.list[]
proto:use `kx.protobuf
show proto.versionStr[];
result:proto.serializeArrayFromList[`ScalarExample; (1i;2.2f;"abc")]
show result;
show proto.parseArrayToList[`ScalarExample; result];
