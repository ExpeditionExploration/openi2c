const { execSync } = require('child_process');

try {
  execSync('mkdir -p build/ref_drv', { stdio: 'inherit' });
  console.log('Created dir build/ref_drv.');

  execSync('cp ref_driver.Makefile build/ref_drv/ref_driver.Makefile', { stdio: 'inherit' });
  console.log('ref_driver.Makefile copied successfully.');

  execSync('make -C build/ref_drv -f ref_driver.Makefile', { stdio: 'inherit' });
  console.log('Reference driver built successfully.');

} catch (error) {
  console.error('Failed to build VL53L5CX Linux reference driver to use:', error);
  process.exit(1);
}
