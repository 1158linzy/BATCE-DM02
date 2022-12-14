/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: 实现CPPTest的flag标识设置
 */

#include "gtest/hwext/gtest-tag.h"
#include "gtest/hwext/gtest-utils.h"

namespace testing {
  namespace ext {

    const TestSizeSet TestSize;
    const TypeSet G_TYPE;
    const SizeSet G_SIZE;
    const RankSet G_RANK;
    const int G_TESTSIZESHIFT = 24;
    const int G_TYPESHIFT = 8;
    const int G_SIZESHIFT = 4;

    TestFlag::TestFlag(const char*  n, const char* d, int m) : name(n), desc(d), mask(m) 
    {
        eleCount = 0;
    }

    void TestFlag::element(const char* desc, int hex) 
    {
        elementMap.insert(pair<int, const char* const>(hex, desc));
    }

    void TestFlag::printHelp(const char** indents) const 
    {
        map<int, const char*>::const_iterator c_iter;
        int i = 0;
        printf("%s--gtest_%s=(", indents[0], naming());
        for (c_iter = elementMap.begin(); c_iter != elementMap.end(); c_iter++) {
            printf("%s%s", (i > 0) ? "|" : "", c_iter->second);
            i++;
        }
        printf(")\n");
        printf("%s%s\n", indents[1], description());
    }

    bool TestFlag::verify(int hex, char* err) const 
    {
        // wipe bits not belong to this set
        int masked = hex&mask;

        map<int, const char*>::const_iterator c_iter;
        for (c_iter = elementMap.begin(); c_iter != elementMap.end(); c_iter++) {
            // wipe each bit defined in this set
            masked &= ~c_iter->first;
        }

        // masked!=0 means find undefined bit(s)
        const bool nice = masked == 0;
        if (!nice&&err != NULL) {
            sprintf(err, "Illegal %s value '0x%08x'\n", naming(), hex);
        }

        return nice;
    }

    bool TestFlag::eleForName(const char* name, int& result) const 
    {
        if (name == NULL) {
            // NOTE:: strlen(NULL) will cause seg fault
            return false;
        }

        const int result_raw = result;
        // must set to 0 here, because we will do bit-OR arithmetic later
        result = 0;
        // the parameter 'name' may contatin more than one element
        const char* kCandidateSeps = ",;|/";
        char name_cpoy[256];
        memset(name_cpoy, 0, 256);
        memcpy(name_cpoy, name, strlen(name));

        char * pch;
        pch = strtok(name_cpoy, kCandidateSeps);
        while (pch != NULL)
        {
            map<int, const char*>::const_iterator c_iter;
            bool matched = false;
            // try to matchNaming all the elements
            for (c_iter = elementMap.begin(); c_iter != elementMap.end(); c_iter++) {
                const char* ele_name = c_iter->second;
                const string ele_full_name_string = string(naming()) + "." + ele_name;
                if (compareStringsByIgnoreCase(pch, ele_name) || compareStringsByIgnoreCase(pch, ele_full_name_string.c_str())) {
                    // set this bit
                    result |= c_iter->first;
                    matched = true;
                    break;
                }
            }
            if (!matched) {
                fprintf(stderr, "Unrecognized %s value '%s'\n", naming(), pch);
                // abort the matching, recover result to raw value
                result = result_raw;
                return false;
            }
            pch = strtok(NULL, kCandidateSeps);
        }
        return true;
    }

    TestSizeSet::TestSizeSet() : TestFlag ("testsize", "Select tests by test level hint.", 0xff << G_TESTSIZESHIFT) 
    {
        element("Level0", Level0);
        element("Level1", Level1);
        element("Level2", Level2);
        element("Level3", Level3);
        element("Level4", Level4);
    }

    TypeSet::TypeSet() : TestFlag ("type", "Select testcase by type hint.", 0x0f << G_TYPESHIFT) 
    {
        element("Function", function);
        element("Performance", performance);
        element("Power", power);
        element("Reliability", reliability);
        element("Security", security);
        element("Global", global);
        element("Compatibility", compatibility);
        element("User", user);
        element("Standard", standard);
        element("Safety", safety);
        element("Resilience", resilience);
    }


    SizeSet::SizeSet() : TestFlag ("size", "Select testcase by size hint.", 0x0f << G_SIZESHIFT) 
    {
        element("SmallTest", smallTest);
        element("MediumTest", mediumTest);
        element("LargeTest", largeTest);
    }


    RankSet::RankSet() : TestFlag("rank", "Select testcase by rank hint.", 0x0f) 
    {
        element("Level0", level0);
        element("Level1", level1);
        element("Level2", level2);
        element("Level3", level3);
        element("Level4", level4);
    }

    static std::vector<const TestFlag*> sets;

    static void ensureSetsInit() {
        if (sets.size() > 0) {
            return;
        }
        sets.push_back(&TestSize);
        sets.push_back(&G_TYPE);
        sets.push_back(&G_SIZE);
        sets.push_back(&G_RANK);
    }

    const std::vector<const TestFlag*>& AllHextTagSets() 
    {
        ensureSetsInit();
        return sets;
    }

    bool CheckFlagsLegality(int flags) 
    {
        ensureSetsInit();
        char buf[256];
        for (unsigned int i = 0; i < sets.size(); i++)
        {
            const TestFlag* set = sets.at(i);
            memset(buf, 0, 256);
            if (!set->verify(flags, buf)) {
                return false;
            }
        }
        return true;
    }

    bool flagForName(const char* set_name, const char* ele_name, int& result) {
        ensureSetsInit();
        for (unsigned int i = 0; i < sets.size(); i++)
        {
            const TestFlag* set = sets.at(i);
            if (!compareStringsByIgnoreCase(set_name, set->naming())) {
                continue;
            }
            return set->eleForName(ele_name, result);
        }
        fprintf(stderr, "Unrecognized flagset '%s'\n", set_name);
        return false;
    }

  } // namespace ext
} // namespace testing