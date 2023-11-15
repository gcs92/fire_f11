#ifndef __PRODUCTION_TEST_H__
#define __PRODUCTION_TEST_H__

typedef struct
{
	unsigned char id;
    void (*cb)(void);
} sPro_Test_TypeDef;

typedef struct
{
	unsigned char id:6;
    unsigned char flag:2;
    void (*cb)(void);
} sPro_Test_DI_TypeDef;

void TEST_UOC_DI0_FUNC(void);
void TEST_UOC_DI1_FUNC(void);
void TEST_UOC_DI2_FUNC(void);
void TEST_UOC_DI3_FUNC(void);
void TEST_UOC_DI4_FUNC(void);
void TEST_UOC_DI5_FUNC(void);
void TEST_UOC_DI6_FUNC(void);
void TEST_UOC_DI7_FUNC(void);
void TEST_UOC_DI8_FUNC(void);
void TEST_UOC_DI9_FUNC(void);
void TEST_UOC_DI10_FUNC(void);
void TEST_UOC_DI11_FUNC(void);
void TEST_UOC_DI12_FUNC(void);
void TEST_UOC_DI13_FUNC(void);
void TEST_UOC_DI14_FUNC(void);

void production_test_main(void);
void production_test_model(void);
void production_test_model1(void);
void production_test_model2(void);
void production_test_model3(void);
void production_test_model4(void);
#endif
