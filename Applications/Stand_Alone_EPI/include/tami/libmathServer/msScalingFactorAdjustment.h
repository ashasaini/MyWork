#ifndef msScalingFactorAdjustment_H__
#define msScalingFactorAdjustment_H__

#ifdef __cplusplus
extern "C" {
#endif

#define SCALE_MAGNITUDE 16384.0f
#define SCALE_REAL_ONLY 8192.0f
#define SCALE_IMGN_ONLY 8192.0f

typedef struct msScalingFactorAdjustment_s {
  int32_t    inDataType;
  int32_t    outDataForm;
  bitfield_t psRecon;
  bool_t     shimFlag;
} msScalingFactorAdjustment_t, *msScalingFactorAdjustment_p;

void msScalingFactorAdjustInit(msScalingFactorAdjustment_t *dataFlag);

void msScalingFactorAdjustment(msScalingFactorAdjustment_t dataFlag,
			       flt32_t                     *scale);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* msScalingFactorAdjustment_H__ */
