#ifdef CONFIG_BOOT_IMAGE_ACCESS_HOOK_NRF52840

#include <assert.h>
#include <zephyr/kernel.h>
#include "bootutil/image.h"
#include "bootutil/bootutil.h"
#include "bootutil/fault_injection_hardening.h"
#include "flash_map_backend/flash_map_backend.h"
#include "io/io.h"
#include "bootutil/bootutil_log.h"

#warning "Compiling my_hooks.c"

bool image_uploaded = false;

extern void do_boot(struct boot_rsp *rsp);

// __attribute__((noreturn)) void do_boot(struct boot_rsp *rsp);

BOOT_LOG_MODULE_DECLARE(mcuboot);

/* @retval 0: header was read/populated
 *         FIH_FAILURE: image is invalid,
 *         BOOT_HOOK_REGULAR if hook not implemented for the image-slot,
 *         othervise an error-code value.
 */
int boot_read_image_header_hook(int img_index, int slot,
                                struct image_header *img_hed)
{
    if (img_index == 1 && slot == 0) {
            img_hed->ih_magic = IMAGE_MAGIC;
            return 0;
    }

    return BOOT_HOOK_REGULAR;
}

/* @retval FIH_SUCCESS: image is valid,
 *         FIH_FAILURE: image is invalid,
 *         fih encoded BOOT_HOOK_REGULAR if hook not implemented for
 *         the image-slot.
 */
fih_ret boot_image_check_hook(int img_index, int slot)
{
    if (img_index == 1 && slot == 0) {
        FIH_RET(FIH_SUCCESS);
    }

    FIH_RET(FIH_BOOT_HOOK_REGULAR);
}

int boot_perform_update_hook(int img_index, struct image_header *img_head,
                             const struct flash_area *area)
{
    if (img_index == 1) {
        return 0;
    }

    return BOOT_HOOK_REGULAR;
}

int boot_read_swap_state_primary_slot_hook(int image_index,
                                           struct boot_swap_state *state)
{
    if (image_index == 1) {
        state->magic = BOOT_MAGIC_UNSET;
        state->swap_type = BOOT_SWAP_TYPE_NONE;
        state->image_num = image_index ; // ?
        state->copy_done = BOOT_FLAG_UNSET;
        state->image_ok = BOOT_FLAG_UNSET;

        return 0;
    }

    return BOOT_HOOK_REGULAR;
}

int boot_copy_region_post_hook(int img_index, const struct flash_area *area,
                               size_t size)
{
    return 0;
}


__attribute__((used))int boot_serial_uploaded_hook(int img_index, const struct flash_area *area,
                               size_t size)
{
    BOOT_LOG_INF("Image uploaded to slot=%d, size=%d", img_index, size);
    image_uploaded = true;

    return 0;
}

int boot_img_install_stat_hook(int image_index, int slot, int *img_install_stat)
{
    return BOOT_HOOK_REGULAR;
}

int boot_reset_request_hook(bool force)
{
	ARG_UNUSED(force);

	return 0;
}

#endif /* CONFIG_BOOT_IMAGE_ACCESS_HOOK_NRF5340 */
