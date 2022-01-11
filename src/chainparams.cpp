// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Copyright (c) 2019-2022 IsotopeC Development Labs
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include "chainparamsseeds.h"
#include "pow.h"
#include "arith_uint256.h"


static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * IsotopeC genesis block 0 transaction cannot be spent.
 * consensus.hashGenesisBlock=0000094f032c3b1ca5e500df384141b75e6081c832f408f67ed517e9762546b3,
 * genesis.hashMerkleRoot=1d4f749ae57afd16b96d9c8543dec6e04a45e7d0480ef4cfcfff1aaa0695f826,
 *
 * Yespower block 1
 * CBlock(hash=a64ad0cfc7ce7dfbb5a4c5265101c7187b0b7ad6ab41f58d0e929b401c7f484c,
 * yespowerpow=0000327c9c53270aed5150e82f70c4d2de4404f4ad3ea019edab4e4018d0b9de,
 * ver=0x20000000,
 * hashPrevBlock=0000094f032c3b1ca5e500df384141b75e6081c832f408f67ed517e9762546b3,
 * hashMerkleRoot=b21e90bec72d59abb4243af6e9e1637a167fc666c7c502d34c492637213dd886,
 * nTime=1572369083, nBits=1e3fffff, nNonce=2147535366, vtx=1)
 * CTransaction(hash=b21e90bec7, ver=2, vin.size=1, vout.size=2, nLockTime=0)
 * CTxIn(COutPoint(0000000000, 4294967295), coinbase 510101)
 * CScriptWitness()
 * CTxOut(nValue=32.00000000, scriptPubKey=21026be22ef501b20c08b21fc33c01)
 * CTxOut(nValue=0.00000000, scriptPubKey=6a24aa21a9ede2f61c3f71d1defd3f)
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    // The tree of
    const char* pszTimestamp = "IsotopeC: Oct/28/2019 liberty must be refreshed from time to time"; // with the blood of patriots and tyrants. It is its natural manure.
    const CScript genesisOutputScript = CScript() << ParseHex("04174140930af0ed9cffd311f86265bcffa0560d811929ce4bdb7e84a49e85881024a727212d6327db1804e4bdf265f4d800ad2c8f83a51fe55377f6c564649968") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
    // Asset for the cause; freedom.
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 420480;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash   = uint256S("0x0000094f032c3b1ca5e500df384141b75e6081c832f408f67ed517e9762546b3");
        consensus.BIP65Height = 0;
        consensus.BIP66Height = 0;
        consensus.powLimit    = uint256S("3fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 7 * 24 * 60 * 60; // 7 days
        consensus.nPowTargetSpacing  = 5 * 60; // 5 minutes
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999;

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1521417600;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1552953600;

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1521417600;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1552953600;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork =  uint256S ("0x00000000000000000000000000000000000000000000000000000145dfd4b29b");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S ("0x0000094f032c3b1ca5e500df384141b75e6081c832f408f67ed517e9762546b3");

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0x4a;
        pchMessageStart[1] = 0x6f;
        pchMessageStart[2] = 0x61;
        pchMessageStart[3] = 0x6e;
        nDefaultPort       = 8675;
        nPruneAfterHeight  = 100000;

        genesis = CreateGenesisBlock(1572248631, 305493, 0x1e0ffff0, 1, 32*COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        
        assert (consensus.hashGenesisBlock == uint256S("0x0000094f032c3b1ca5e500df384141b75e6081c832f408f67ed517e9762546b3"));
        assert (genesis.hashMerkleRoot == uint256S("0x1d4f749ae57afd16b96d9c8543dec6e04a45e7d0480ef4cfcfff1aaa0695f826"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,102);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,88);
        base58Prefixes[SECRET_KEY]     = std::vector<unsigned char>(1,128);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));
        vSeeds.clear();
        vSeeds.emplace_back("seed.isotopec.org", false); // IsotopeC Development Labs
        vSeeds.emplace_back("node.isotopec.org", false); // IsotopeC Development Labs

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fMiningRequiresPeers = true;

        checkpointData = (CCheckpointData) {
            {
                { 0,        uint256S("0x0000094f032c3b1ca5e500df384141b75e6081c832f408f67ed517e9762546b3") },
                { 10000,    uint256S("0xfdce81dcc439576a4dea1548540c091341b8cea24f8d259932f6ffb8b9fc77c8") },
                { 20000,    uint256S("0x2f8488879cf7ab97320ebbe8c381afda1066b91c001230e338c2e5ee9ebd8cce") },
                { 30000,    uint256S("0xdbb13ef1d428a614d28eb1b48c85fcb52cb287a30a17ca4873ffde92ca30a056") },
                { 40000,    uint256S("0x43fd0256f9c6a12d92ae6d05449fa0271de82afc662b52847d10f5c8e3c7c7e0") },
                { 50000,    uint256S("0x439fe004459df67dfb02189893da9b9323f3f98bfd4c71aa4f208a2c2bd8ecc4") },
                { 60000,    uint256S("0xf8c0f5145ab9345729907c55084536032f0da39734322de2470e693143713cbb") },
                { 70000,    uint256S("0xade731fff6f0f6bfd7ab84df104b5e69d09fe5e1de14fb33b70b210a0ea2cfa9") },
                { 80000,    uint256S("0x7774e4c3493595208fc42d5045f0b9923eaea07b28c2ac070e199a8ae92e273d") },
                { 90000,    uint256S("0x7619a3fa511c721b929e9fa45fccf42486bcd5d953a4c0bab705b9e52859725f") },
                { 100000,   uint256S("0x9e17b88a306c397650ca3bbe803e87cf085c11cc4bc3065e2c7f631e9a7eb443") },
                { 110000,   uint256S("0xaea334c720f14d9fc6c6cd0df655f4ba1884fedb885b93fd18076267671eea02") },
                { 120000,   uint256S("0xe13e84ec5a3eedb5e5a709ef4636579976398907e2c1abb3856454a8649d51cf") },
                { 130000,   uint256S("0x7f1d9e90497ae92d20f9bd3852719f97125d3dc83d8db00a3ca69774a904af16") },
                { 140000,   uint256S("0x73b5839debbcb0d58970aa64c81c5de8999e98fd15c54fafe0d6369ef3336c40") },
                { 150000,   uint256S("0x348fc164383a061bbebc37f973df8dd28397919c62a6017beb33ec9d13deccbc") },
                { 160000,   uint256S("0xb4fda8f56e4c1f468f9f9efedc3f3dd66416c5a42bbdc5252ffe6ff25c0418d0") },
                { 170000,   uint256S("0x4e0bc54214de9c811861e437b076663f746abd65f8247f2c5ef21431264f2052") },
                { 180000,   uint256S("0x0cf39ed0c3e1ec5e6e519c2ebc1b4bfd3e9f50554b3ebdd85c81d2a32a95a4cd") },
                { 190000,   uint256S("0x265448a5199c8d4a0f2fd943f0f5cde5992f001d3c0dbf1f5817bb9b57cce8f6") },
                { 200000,   uint256S("0x0134209f3d1d1ebfd7a7c430b7e6606c0e6dc88775889d87d6364e929902fc31") },
            }
        };

        chainTxData = ChainTxData {
            // Data as of 0x7b04a3ec71bd59ca772c2aee02d43327d91dd38e8923a79da844813afa921c32 // Block 207750
            1641878228,          // * UNIX timestamp of last known number of transactions
            291883,              // * total number of transactions between genesis and that timestamp
                                 //   (the tx=... number in the SetBestChain debug.log lines)
            0.004201061548932724 // * estimated number of transactions per second after that timestamp
        };
    }
};

/**
 * Testnet (v1)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 420480;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash   = uint256S("0x0000094f032c3b1ca5e500df384141b75e6081c832f408f67ed517e9762546b3");
        consensus.BIP65Height = 0;
        consensus.BIP66Height = 0;
        consensus.powLimit    = uint256S("3fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 7 * 24 * 60 * 60; // 7 days
        consensus.nPowTargetSpacing  = 5 * 60; // 5 minutes
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1521417600; // March 19th, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1552953600; // March 19th, 2019

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1521417600; // March 19th, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1552953600; // March 19th, 2019

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork =  uint256S ("0x0000000000000000000000000000000000000000000000000000000000000000");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S ("0x0000094f032c3b1ca5e500df384141b75e6081c832f408f67ed517e9762546b3");

        pchMessageStart[0] = 0x46;
        pchMessageStart[1] = 0x4b;
        pchMessageStart[2] = 0x65;
        pchMessageStart[3] = 0x6e;
        nDefaultPort       = 18675;
        nPruneAfterHeight  = 1000;

        genesis = CreateGenesisBlock(1572248631, 305493, 0x1e0ffff0, 1, 32*COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert (consensus.hashGenesisBlock == uint256S("0x0000094f032c3b1ca5e500df384141b75e6081c832f408f67ed517e9762546b3"));
        assert (genesis.hashMerkleRoot == uint256S("0x1d4f749ae57afd16b96d9c8543dec6e04a45e7d0480ef4cfcfff1aaa0695f826"));

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));
        vSeeds.emplace_back("testnet.isotopec.org", false); // IsotopeC Development Labs

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,43);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,45);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,199);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fMiningRequiresPeers = true;

        checkpointData = (CCheckpointData) {
            {
                {0, uint256S("0x0000094f032c3b1ca5e500df384141b75e6081c832f408f67ed517e9762546b3")},
            }
        };

        chainTxData = ChainTxData {
            /* data as of:  */
            0,
            0,
            0.0
        };
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 15;
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 0; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 0; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("3fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 7 * 24 * 60 * 60; // 7 days
        consensus.nPowTargetSpacing  = 5 * 60; // 5 minutes
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork  = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x0000094f032c3b1ca5e500df384141b75e6081c832f408f67ed517e9762546b3");

        pchMessageStart[0] = 0x6e;
        pchMessageStart[1] = 0x65;
        pchMessageStart[2] = 0x64;
        pchMessageStart[3] = 0x79;
        nDefaultPort       = 15309;
        nPruneAfterHeight  = 1000;

        genesis = CreateGenesisBlock(1572248631, 305493, 0x1e0ffff0, 1, 32*COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert (consensus.hashGenesisBlock == uint256S("0x0000094f032c3b1ca5e500df384141b75e6081c832f408f67ed517e9762546b3"));
        assert (genesis.hashMerkleRoot == uint256S("0x1d4f749ae57afd16b96d9c8543dec6e04a45e7d0480ef4cfcfff1aaa0695f826"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fMiningRequiresPeers = false;

        checkpointData = (CCheckpointData) {
            {
                {0, uint256S("0x0000094f032c3b1ca5e500df384141b75e6081c832f408f67ed517e9762546b3")},
            }
        };

        chainTxData = ChainTxData {
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,43);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,45);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,199);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}
